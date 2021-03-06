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

#include <set>

namespace physics {

class DynamicBody {
public:

	enum class TransformSpace {
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
	glm::vec3 scale() const { return transform_.scale_; }

	void setPosition(glm::vec3 pos, TransformSpace space);
	void setOrientation(glm::fquat o, TransformSpace space);
	void setScale(float scale);
	void setScale(glm::vec3 scale);

	void applyForce(TransformSpace space, glm::vec3 offset, glm::vec3 forceVector);

private:
	DynamicBody* parent_ = nullptr;
	std::set<DynamicBody*> children_;
	struct {
		glm::vec3 position_ {0};
		glm::fquat orientation_ {};
		glm::vec3 scale_ {1.f};
	} transform_; // transformation from model space to parent space

	mutable glm::mat4 matFrameCache_ {1}; // cached frame transformation from model space to parent space
	mutable glm::mat4 matWorldCache_ {1}; // cached world space transformation
	mutable bool frameTransformDirty_ = false;
	mutable bool wldTransformDirty_ = false;

	void computeFrameTransform() const;
	glm::mat4 worldTransform() const;

	void removeChild(DynamicBody* c);
};

} /* namespace physics */

#endif /* PHYSICS_DYNAMICBODY_H_ */
