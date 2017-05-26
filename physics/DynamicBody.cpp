/*
 * DynamicBody.cpp
 *
 *  Created on: Apr 27, 2017
 *      Author: bog
 */

#include "DynamicBody.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/transform.hpp>

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
		if (frameTransformDirty_)
			computeFrameTransform();
		return matFrameCache_;
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
		if (frameTransformDirty_)
			computeFrameTransform();
		if (parent_)
			matWorldCache_ = parent_->worldTransform() * matFrameCache_;
		else
			matWorldCache_ = matFrameCache_;
		wldTransformDirty_ = false;
	}
	return matWorldCache_;
}

glm::vec3 DynamicBody::getPosition(TransformSpace space) const {
	if (space == TransformSpace::World) {
		return glm::vec3(worldTransform()[3]);
	}
	return transform_.position_;
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
		return transform_.orientation_;
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
		// transform pos from world into parents
		if (parent_) {
			auto mInv = glm::inverse(parent_->worldTransform());
			pos = glm::vec3{mInv * glm::vec4{pos, 1}};
		}
		break;
	default:
		assert(!"invalid space");
	}
	transform_.position_ = pos;
	frameTransformDirty_ = wldTransformDirty_ = true;
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
	transform_.orientation_ = o;
	frameTransformDirty_ = wldTransformDirty_ = true;
}

void DynamicBody::computeFrameTransform() const {
	matFrameCache_ = glm::scale(transform_.scale_) * glm::mat4_cast(transform_.orientation_);
	matFrameCache_[3] = {transform_.position_, 1};
	frameTransformDirty_ = false;
}

void DynamicBody::setScale(glm::vec3 s) {
	transform_.scale_ = s;
	frameTransformDirty_ = wldTransformDirty_ = true;
}

void DynamicBody::setScale(float s) {
	setScale({s ,s ,s });
}
