/*
 * Camera.cpp
 *
 *  Created on: Nov 9, 2014
 *      Author: bog
 */

#include "Camera.h"
#include "Viewport.h"
#include "../math/math3D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

Camera::Camera(Viewport* vp)
	: pViewport_(vp)
	, fov_(PI/2)
	, matView_(1)
	, matProj_(1)
{
	updateProj();
}

Camera::~Camera() {
}

void Camera::setFOV(float fov) {
	fov_ = fov;
	updateProj();
}

void Camera::moveTo(glm::vec3 const& wWhere) {
	matView_[3][0] = -wWhere.x;
	matView_[3][1] = -wWhere.y;
	matView_[3][2] = -wWhere.z;
}

void Camera::updateProj() {
	float zNear = 0.5f;
	float zFar = 50.f;
	matProj_ = glm::perspectiveFovLH(fov_, (float)pViewport_->getWidth(), (float)pViewport_->getHeight(), zNear, zFar);
	matProj_ = glm::translate(glm::vec3(pViewport_->getPosition(), 0)) * matProj_;
}

