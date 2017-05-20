/*
 * PathController.h
 *
 *  Created on: May 21, 2017
 *      Author: bogdan
 */

#ifndef PHYSICS_PATHCONTROLLER_H_
#define PHYSICS_PATHCONTROLLER_H_

#include "Entity.h"

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class DynamicBody;

class PathController : public EntityController {
public:

	struct Vertex {
		glm::vec3 position;
		glm::fquat orientation;

		Vertex operator+(Vertex x) const {
			return {position + x.position, orientation + x.orientation};
		}
		float operator-(Vertex x) const {
			float d = (position - x.position).length();
			return d == 0 ? 1.f : d;
		}
	};

	PathController(DynamicBody *body);
	virtual ~PathController();

	void addVertex(Vertex v);
	void start(float speed);

	void update(float dt);
};

#endif /* PHYSICS_PATHCONTROLLER_H_ */
