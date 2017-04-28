/*
 * DynamicBody.h
 *
 *  Created on: Apr 27, 2017
 *      Author: bog
 */

#ifndef PHYSICS_DYNAMICBODY_H_
#define PHYSICS_DYNAMICBODY_H_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace physics {

class DynamicBody {
public:

	enum class TransformSpace {
		Local,
		Parent,
		World,
		Custom,
	};

	DynamicBody();
	virtual ~DynamicBody();

	void setParent(DynamicBody* parent);

	void update(float dt);

	glm::mat4 getTransformation(TransformSpace space, const glm::mat4* customSpace = nullptr) const;
	glm::vec3 getPosition(TransformSpace space) const;
	glm::vec3 getSpeed(TransformSpace space) const;
	glm::vec3 getAcceleration(TransformSpace space) const;
	glm::fquat getOrientation(TransformSpace space) const;
	glm::fquat getAngularSpeed(TransformSpace space) const;
	glm::fquat getAngularAcceleration(TransformSpace space) const;

	void applyForce(TransformSpace space, glm::vec3 offset, glm::vec3 forceVector);
};

} /* namespace physics */

#endif /* PHYSICS_DYNAMICBODY_H_ */
