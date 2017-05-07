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
	glm::vec3 trans { matView_[3][0], matView_[3][1], matView_[3][2] };
	matView_ = glm::lookAtLH(-trans, where, {0, 1, 0});
	moveTo(-trans);
}

void Camera::updateProj() {
	float zNear = 0.5f;
	float zFar = 50.f;
	matProj_ = glm::perspectiveFovLH(fov_, (float)pViewport_->width(), (float)pViewport_->height(), zNear, zFar);
}

