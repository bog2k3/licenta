/*
 * World.h
 *
 *  Created on: Nov 12, 2014
 *      Author: bogdan
 */

#ifndef WORLD_H_
#define WORLD_H_

#include "entities/Entity.h"
#include "utils/MTVector.h"
#include "renderOpenGL/RenderContext.h"

#include <vector>
#include <memory>
#include <atomic>

class World {
public:
	static World* getInstance();
	World();
	virtual ~World();

	/**
	 * delete all entities and reset state.
	 * set new world spatial extents
	 */
	void reset();

	//void setBounds(float left, float right, float top, float bottom);

	//b2Body* getBodyAtPos(glm::vec2 const& pos) override;
	//void getBodiesInArea(glm::vec2 const& pos, float radius, bool clipToCircle, std::vector<b2Body*> &outBodies);

	//void setPhysics(b2World* physWld);
	//void setDestroyListener(PhysDestroyListener *listener) { destroyListener_ = listener; }
	//PhysDestroyListener* getDestroyListener() { return destroyListener_; }
	//b2World* getPhysics() { return physWld; }
	//b2Body* getGroundBody() { return groundBody; }

	void takeOwnershipOf(std::unique_ptr<Entity> &&e);
	void destroyEntity(Entity* e);

	// get all entities that match ALL of the requested features
	void getEntities(std::vector<Entity*> &out, int filterTypes, Entity::FunctionalityFlags filterFlags = Entity::FunctionalityFlags::NONE);

	// get all entities in a specific area that match ALL of the requested features
	void getEntitiesInBox(std::vector<Entity*> &out, int filterTypes, Entity::FunctionalityFlags filterFlags, glm::vec2 const& pos, float radius, bool clipToCircle);

	void update(float dt);
	void draw();

	// this is thread safe by design; if called from the synchronous loop that executes deferred actions, it's executed immediately, else added to the queue
	void queueDeferredAction(std::function<void()> &&fun);

#ifdef DEBUG
	static void assertOnMainThread() {
		assert(std::this_thread::get_id() == getInstance()->ownerThreadId_);
	}
#endif

protected:
	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<Entity*> entsToUpdate;
	std::vector<Entity*> entsToDraw;
	MTVector<Entity*> entsToDestroy;
	MTVector<std::unique_ptr<Entity>> entsToTakeOver;
	int frameNumber_ = 0;
#ifdef DEBUG
	std::thread::id ownerThreadId_;
#endif

	// this holds actions deferred from the multi-threaded update which will be executed synchronously at the end on a single thread
	MTVector<std::function<void()>> deferredActions_;
	std::atomic<bool> executingDeferredActions_ { false };

	void destroyPending();
	void takeOverPending();

	bool testEntity(Entity &e, int filterTypes, Entity::FunctionalityFlags filterFlags);
};

#endif /* WORLD_H_ */

