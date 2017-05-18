/*
 * Camera.cpp
 *
 *  Created on: Nov 9, 2014
 *      Author: bog
 */

#include "Camera.h"
#include "Viewport.h"
#include "../math/math3D.h"

#include "../utils/log.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

Camera::Camera(Viewport* vp)
	: pViewport_(vp)
	, fov_(PI/2)
	, matView_(1)
	, matProj_(1)
	, position_(0)
	, direction_(0, 0, 1)
{
	updateProj();
}

Camera::~Camera() {
}

void Camera::setFOV(float fov) {
	fov_ = fov;
	updateProj();
}

void Camera::setOrtho(glm::vec4 rc) {
	ortho_ = rc;
	fov_ = 0;
	updateProj();
}

void Camera::moveTo(glm::vec3 const& where) {
	position_ = where;
	updateView();
}

void Camera::lookAt(glm::vec3 const& where) {
	direction_ = glm::normalize(where - position_);
	updateView();
}

void Camera::updateView() {
	matView_ = glm::lookAtLH(position_, position_ + direction_, {0, 1, 0});
}

void Camera::updateProj() {
	float zNear = 0.5f;
	float zFar = 50.f;
	if (fov_ == 0) {
		// set ortho
		matProj_ = glm::orthoLH(ortho_.x, ortho_.x + ortho_.z, ortho_.y, ortho_.y+ortho_.w, zNear, zFar);
	} else {
		// set perspective
		matProj_ = glm::perspectiveFovLH(fov_, (float)pViewport_->width(), (float)pViewport_->height(), zNear, zFar);
	}
}

