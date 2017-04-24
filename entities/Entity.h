/*
 * Entity.h
 *
 *  Created on: Jan 21, 2015
 *      Author: bog
 */

#ifndef ENTITIES_ENTITY_H_
#define ENTITIES_ENTITY_H_

#include "ent-types.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <atomic>

class RenderContext;
//struct aabb;

class Entity {
public:
	virtual ~Entity();

	enum class FunctionalityFlags {
		NONE			= 0,
		DONT_CARE		= 0,
		DRAWABLE		= 1,
		UPDATABLE		= 2,
		SERIALIZABLE	= 4,
	};

	virtual int getEntityType() const = 0;
	// these flags MUST NOT change during the life time of the object, or else UNDEFINED BEHAVIOUR
	virtual FunctionalityFlags getFunctionalityFlags() const { return FunctionalityFlags::NONE; }

	virtual void update(float dt) { assert(!"Forgot to override?"); }
	virtual void draw(RenderContext const& ctx) { assert(!"Forgot to override?"); }

	void destroy();
	bool isZombie() const { return markedForDeletion_.load(std::memory_order_acquire); }

protected:
	Entity() = default;

private:
	std::atomic<bool> markedForDeletion_ {false};
	bool managed_ = false;
	friend class World;
};

#endif /* ENTITIES_ENTITY_H_ */
