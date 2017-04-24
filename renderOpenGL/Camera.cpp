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

Camera::Camera(Viewport* vp)
	: pViewport_(vp)
	, fov_(PI/2)
	, matView_(1)
	, matProj_(1)
{
}

Camera::~Camera() {
}

void Camera::setFOV(float fov) {
	fov_ = fov; updateProj();
}

void Camera::moveTo(glm::vec3 const& wWhere) {
	matView_[0][3] = wWhere.x;
	matView_[1][3] = wWhere.y;
	matView_[2][3] = wWhere.z;
}

void Camera::updateProj() {
	float zNear = 0.5f;
	float zFar = 50.f;
	matProj_ = glm::perspectiveFovLH(fov_, (float)pViewport_->getWidth(), (float)pViewport_->getHeight(), zNear, zFar);
}

