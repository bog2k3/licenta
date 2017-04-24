/*
 * Camera.h
 *
 *  Created on: Nov 9, 2014
 *      Author: bog
 */

#ifndef RENDEROPENGL_CAMERA_H_
#define RENDEROPENGL_CAMERA_H_

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Viewport;

class Camera {
public:
	Camera(Viewport* vp);
	virtual ~Camera();

	const glm::mat4& getMatView() const { return matView_; }
	const glm::mat4 getMatViewProj() const { return matProj_ * matView_; }

	glm::vec3 getPos() const;
	void moveTo(glm::vec3 const& wWhere);
	void transformView(glm::mat4 const& rTrans);
	void setViewTransform(glm::mat4 aTrans);
	float getFOV() const { return fov_; }
	void setFOV(float fov);

protected:
	Viewport* pViewport_;
	float fov_;
	glm::mat4 matView_;
	glm::mat4 matProj_;

	void updateProj();

	friend class Viewport;
};

#endif /* RENDEROPENGL_CAMERA_H_ */
