/*
 * Camera.h
 *
 *  Created on: Nov 9, 2014
 *      Author: bog
 */

#ifndef RENDEROPENGL_CAMERA_H_
#define RENDEROPENGL_CAMERA_H_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class Viewport;

class Camera {
public:
	Camera(Viewport* vp);
	virtual ~Camera();

	const glm::mat4& matView() const { return matView_; }
	const glm::mat4& matProj() const { return matProj_; }
	const glm::mat4 matProjView() const { return matProj_ * matView_; }

	glm::vec3 position() const { return position_; }
	glm::vec3 direction() const { return direction_; }
	void moveTo(glm::vec3 const& where);
	void lookAt(glm::vec3 const& where);
	void transformView(glm::mat4 const& rTrans);
	void setViewTransform(glm::mat4 aTrans);
	float FOV() const { return fov_; }
	void setFOV(float fov);
	void setOrtho(glm::vec4 rect); // sets ortho projection - x,y=pos, z,w=size
	glm::vec4 getOrthoRect() const { return ortho_; }

protected:
	Viewport* pViewport_;
	float fov_;
	glm::mat4 matView_;
	glm::mat4 matProj_;
	glm::vec3 position_;
	glm::vec3 direction_;
	glm::vec4 ortho_;

	void updateView();
	void updateProj();

	friend class Viewport;
};

#endif /* RENDEROPENGL_CAMERA_H_ */
