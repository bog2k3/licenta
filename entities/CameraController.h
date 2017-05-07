/*
 * CameraController.h
 *
 *  Created on: May 7, 2017
 *      Author: bog
 */

#ifndef ENTITIES_CAMERACONTROLLER_H_
#define ENTITIES_CAMERACONTROLLER_H_

#include "Entity.h"
#include "ent-types.h"

#include <glm/vec3.hpp>

#include <vector>

class Camera;

class CameraController: public Entity {
public:
	CameraController(Camera* target);
	virtual ~CameraController();

	virtual int getEntityType() const override { return EntTypes::CAMERA_CTRL; }
	// these flags MUST NOT change during the life time of the object, or else UNDEFINED BEHAVIOUR
	virtual FunctionalityFlags getFunctionalityFlags() const override { return FunctionalityFlags::UPDATABLE; }

	virtual void update(float dt) override;

	// starts a back-and-forth movement between p1 and p2. starts from the current position toward p1 first.
	void startBackAndForth(glm::vec3 p1, glm::vec3 p2, glm::vec3 lookAtTarget, float speed);

	struct pathNode {
		enum {
			vertex,		// regular path vertex (will flow towards the next vertex unless jump or redirect or end of path)
			jump,		// abrupt jump to a path vertex
			redirect,	// smooth redirect towards a path vertex
		} type;
		glm::vec3 position;
		glm::vec3 lookAtTarget;
		unsigned targetIndex; // for jump or redirect
	};

	void startPath(std::vector<pathNode> path, float speed);

private:
	Camera* camera_ = nullptr;
	std::vector<pathNode> path_;
	float cruiseSpeed_ = 0;
	int pathIndex_ = -1;
	glm::vec3 origin_{0};
	glm::vec3 originLookAt_{0};
	glm::vec3 lastLookAt_{0};
	float segmentLength_ = 0;
	float lerpFactor_ = 0;
	float lerpSpeed_ = 0;
	float maxLerpSpeed_ = 0;
	bool jumpNext_ = false;
};

#endif /* ENTITIES_CAMERACONTROLLER_H_ */
