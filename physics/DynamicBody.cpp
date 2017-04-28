/*
 * DynamicBody.cpp
 *
 *  Created on: Apr 27, 2017
 *      Author: bog
 */

#include "DynamicBody.h"

using namespace physics;

DynamicBody::DynamicBody() {
	// TODO Auto-generated constructor stub
}

DynamicBody::~DynamicBody() {
	// TODO Auto-generated destructor stub
}

glm::mat4 DynamicBody::getTransformation(TransformSpace space, const glm::mat4* customSpace) const {
	return glm::mat4(1);
}

glm::vec3 DynamicBody::getPosition(TransformSpace space) const {
	return glm::vec3(0);
}

glm::vec3 DynamicBody::getSpeed(TransformSpace space) const {
	return glm::vec3(0);
}

glm::vec3 DynamicBody::getAcceleration(TransformSpace space) const {
	return glm::vec3(0);
}

glm::fquat DynamicBody::getOrientation(TransformSpace space) const {
	return glm::fquat();
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
