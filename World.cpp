/*
 * World.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: bogdan
 */

#include "World.h"
#include "entities/Entity.h"
#include "math/math3D.h"
#include "Infrastructure.h"
#include "renderOpenGL/Shape2D.h"

#include "utils/bitFlags.h"
#include "utils/parallel.h"
#include "utils/assert.h"
#include "utils/log.h"

#include <glm/glm.hpp>
#include <algorithm>

#ifdef DEBUG_DMALLOC
#include <dmalloc.h>
#endif

#define MT_UPDATE	// enables parallel update on entities, using the thread pool

static World *instance = nullptr;

World::World()
	: entsToDestroy(1024)
	, entsToTakeOver(1024)
	, deferredActions_(4096)
{
	assert(instance == nullptr && "attempting to initialize multiple instances of World!!!");
	instance = this;
#ifdef DEBUG
	ownerThreadId_ = std::this_thread::get_id();
#endif
}

World* World::getInstance() {
	assert(instance && "No existing World instance!!!");
	return instance;
}

World::~World() {
	reset();
}

void World::reset() {
	for (auto &e : entities) {
		e->markedForDeletion_= true;
		e.reset();
	}
	for (auto &e : entsToTakeOver) {
		e->markedForDeletion_ = true;
		e.reset();
	}
	entities.clear();
	entsToTakeOver.clear();
	entsToDestroy.clear();
	entsToDraw.clear();
	entsToUpdate.clear();
}

void World::takeOwnershipOf(std::unique_ptr<Entity> &&e) {
	assertDbg(e != nullptr);
	e->managed_ = true;
	entsToTakeOver.push_back(std::move(e));
}

void World::destroyEntity(Entity* e) {
	entsToDestroy.push_back(e);
}

void World::destroyPending() {
	static decltype(entsToDestroy) destroyNow(entsToDestroy.getLockFreeCapacity());
	destroyNow.swap(entsToDestroy);
	for (auto &e : destroyNow) {
		auto it = std::find_if(entities.begin(), entities.end(), [&] (auto &it) {
			return it.get() == e;
		});
		if (it != entities.end()) {
			Entity::FunctionalityFlags flags = e->getFunctionalityFlags();
			if ((flags & Entity::FunctionalityFlags::UPDATABLE) != 0) {
				auto it = std::find(entsToUpdate.begin(), entsToUpdate.end(), e);
				assertDbg(it != entsToUpdate.end());
				entsToUpdate.erase(it);
			}
			if ((flags & Entity::FunctionalityFlags::DRAWABLE) != 0) {
				auto it = std::find(entsToDraw.begin(), entsToDraw.end(), e);
				assertDbg(it != entsToDraw.end());
				entsToDraw.erase(it);
			}
			entities.erase(it); // this will also delete
//TODO optimize this, it will be O(n^2) - must move the pointer from entities to entsToDestroy when destroy()
		} else {
			auto it2 = std::find_if(entsToTakeOver.begin(), entsToTakeOver.end(), [&] (auto &it) {
				return it.get() == e;
			});
			if (it2 != entsToTakeOver.end()) {
				(*it2).reset();
			} else {
				ERROR("[WARNING] World skip DESTROY unmanaged obj: "<<e);
			}
		}
	}
	destroyNow.clear();
}

void World::takeOverPending() {
	static decltype(entsToTakeOver) takeOverNow(entsToTakeOver.getLockFreeCapacity());
	takeOverNow.swap(entsToTakeOver);
	for (auto &e : takeOverNow) {
		if (!e)
			continue;	// entity was destroyed in the mean time
		// add to update and draw lists if appropriate
		Entity::FunctionalityFlags flags = e->getFunctionalityFlags();
		if ((flags & Entity::FunctionalityFlags::DRAWABLE) != 0) {
			entsToDraw.push_back(e.get());
		}
		if ((flags & Entity::FunctionalityFlags::UPDATABLE) != 0) {
			entsToUpdate.push_back(e.get());
		}
		entities.push_back(std::move(e));
	}
	takeOverNow.clear();
}

void World::update(float dt) {
	++frameNumber_;

	// delete pending entities:
	destroyPending();

	// take over pending entities:
	takeOverPending();

	// do the actual update on entities:
	do {
#ifdef MT_UPDATE
	parallel_for(
#else
	std::for_each(
#endif
			entsToUpdate.begin(), entsToUpdate.end(),
#ifdef MT_UPDATE
			Infrastructure::getThreadPool(),
#endif
			[dt] (auto &e) {
				e->update(dt);
			});
	} while (0);

	// execute deferred actions synchronously:
	executingDeferredActions_.store(true, std::memory_order_release);
	for (auto &a : deferredActions_)
		a();
	deferredActions_.clear();
	executingDeferredActions_.store(false, std::memory_order_release);
}

void World::queueDeferredAction(std::function<void()> &&fun) {
	if (executingDeferredActions_)
		fun();
	else
		deferredActions_.push_back(std::move(fun));
}

void World::draw(RenderContext const& ctx) {
	// draw entities
	for (auto e : entsToDraw)
		e->draw(ctx);
}

bool World::testEntity(Entity &e, int filterTypes, Entity::FunctionalityFlags filterFlags) {
	return (e.getEntityType() & filterTypes) != 0 &&
		((e.getFunctionalityFlags() & filterFlags) == filterFlags);
}


void World::getEntities(std::vector<Entity*> &out, int filterTypes, Entity::FunctionalityFlags filterFlags) {
	for (auto &e : entities) {
		if (!e->isZombie() && testEntity(*e, filterTypes, filterFlags))
			out.push_back(e.get());
	}
	for (auto &e : entsToTakeOver) {
		if (!e->isZombie() && testEntity(*e, filterTypes, filterFlags))
			out.push_back(e.get());
	}
}

void World::getEntitiesInBox(std::vector<Entity*> &out, int filterTypes, Entity::FunctionalityFlags filterFlags,
		glm::vec2 const& pos, float radius, bool clipToCircle)
{
	/*spatialCache_.getCachedEntities(out, pos, radius, clipToCircle, frameNumber_,
		[this, filterTypes, filterFlags] (glm::vec2 const& pos, float radius, std::vector<Entity*> &out)
	{
		static thread_local std::vector<b2Body*> bodies;
		bodies.clear();
		getBodiesInArea(pos, radius, false, bodies);
		for (b2Body* b : bodies) {
			PhysicsBody* pb = PhysicsBody::getForB2Body(b);
			if (pb == nullptr)
				continue;
			Entity* ent = pb->getAssociatedEntity();
			if (ent && !ent->isZombie())
				out.push_back(ent);
		}
	}, [this, filterTypes, filterFlags] (Entity *e) {
		return testEntity(*e, filterTypes, filterFlags);
	});*/
}

