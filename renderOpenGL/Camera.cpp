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

void Camera::moveTo(glm::vec3 const& where) {
	matView_[3][0] = -where.x;
	matView_[3][1] = -where.y;
	matView_[3][2] = -where.z;
}

void Camera::lookAt(glm::vec3 const& where) {
	glm::vec3 pos = position();
	matView_ = glm::lookAtLH(pos, where, {0, 1, 0});
	moveTo(pos);
}

void Camera::updateProj() {
	float zNear = 0.5f;
	float zFar = 50.f;
	matProj_ = glm::perspectiveFovLH(fov_, (float)pViewport_->width(), (float)pViewport_->height(), zNear, zFar);
}

glm::vec3 Camera::position() const {
	return {
		-matView_[3][0], -matView_[3][1], -matView_[3][2]
	};
}

glm::vec3 Camera::direction() const {
	return {
		matView_[0][2], matView_[1][2], matView_[2][2]
	};
}
