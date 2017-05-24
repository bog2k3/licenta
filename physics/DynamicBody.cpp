/*
 * DynamicBody.cpp
 *
 *  Created on: Apr 27, 2017
 *      Author: bog
 */

#include "DynamicBody.h"

#include <glm/gtc/matrix_inverse.hpp>

using namespace physics;

DynamicBody::DynamicBody() {
	// TODO Auto-generated constructor stub
}

DynamicBody::~DynamicBody() {
	// TODO Auto-generated destructor stub
}

glm::mat4 DynamicBody::getTransformation(TransformSpace space, const glm::mat4* customSpace) const {
	switch (space) {
	case TransformSpace::Parent:
		return matFrame_;
	case TransformSpace::World:
		return worldTransform();
	case TransformSpace::Custom:
		assert(!"not implemented");
		return glm::mat4{1};
	default:
		assert(!"invalid space");
		return glm::mat4{1};
	}
}

glm::mat4 DynamicBody::worldTransform() const {
	if (wldTransformDirty_) {
		if (parent_)
			matWorldCache_ = parent_->worldTransform() * matFrame_;
		else
			matWorldCache_ = matFrame_;
		wldTransformDirty_ = false;
	}
	return matWorldCache_;
}

glm::vec3 DynamicBody::getPosition(TransformSpace space) const {
	auto w = matFrame_;
	if (space == TransformSpace::World) {
		w = worldTransform();
	}
	return {w[3][0], w[3][1], w[3][2]};
}

glm::vec3 DynamicBody::getSpeed(TransformSpace space) const {
	return glm::vec3{0};
}

glm::vec3 DynamicBody::getAcceleration(TransformSpace space) const {
	return glm::vec3{0};
}

glm::fquat DynamicBody::getOrientation(TransformSpace space) const {
	switch (space) {
	case TransformSpace::Parent:
		return glm::quat_cast(matFrame_);
	case TransformSpace::World:
		return glm::quat_cast(worldTransform());
	default:
		assert(!"invalid space");
		return glm::fquat();
	}
}

glm::fquat DynamicBody::getAngularSpeed(TransformSpace space) const {
	return glm::fquat();
}

glm::fquat DynamicBody::getAngularAcceleration(TransformSpace space) const {
	return glm::fquat();
}

void DynamicBody::applyForce(TransformSpace space, glm::vec3 offset, glm::vec3 forceVector) {

}

void DynamicBody::update(float dt) {

}

void DynamicBody::setPosition(glm::vec3 pos, TransformSpace space) {
	switch (space) {
	case TransformSpace::Parent:
		break;
	case TransformSpace::World:
		// transform pos from world into parent
		if (parent_) {
			auto mInv = glm::inverse(parent_->worldTransform());
			pos = glm::vec3{mInv * glm::vec4{pos, 1}};
		}
		break;
	default:
		assert(!"invalid space");
	}
	matFrame_[3] = glm::vec4{pos, 1};
	wldTransformDirty_ = true;
}

void DynamicBody::setOrientation(glm::fquat o, TransformSpace space) {
	switch (space) {
	case TransformSpace::Parent:
		break;
	case TransformSpace::World:
		// transform pos from world into parent
		if (parent_) {
			auto mInv = glm::inverse(parent_->worldTransform());
			o = glm::quat_cast(mInv) * o;
		}
		break;
	default:
		assert(!"invalid space");
	}
	auto pos = getPosition(TransformSpace::Parent);
	matFrame_ = glm::mat4_cast(o);
	matFrame_[3] = glm::vec4{pos, 1};
	wldTransformDirty_ = true;
}
