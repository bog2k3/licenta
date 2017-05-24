/*
 * PathController.h
 *
 *  Created on: May 21, 2017
 *      Author: bogdan
 */

#ifndef PHYSICS_PATHCONTROLLER_H_
#define PHYSICS_PATHCONTROLLER_H_

#include "Entity.h"
#include "../utils/path-lerper.h"

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace physics {
class DynamicBody;
}

class PathController : public Entity {
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
		Vertex operator*(float f) const {
			return {position * f, glm::fquat{orientation.x, orientation.y, orientation.z, orientation.w * f}};
		}
	};

	int getEntityType() const override { return EntTypes::PATH_CONTROLLER; }
	FunctionalityFlags getFunctionalityFlags() const { return FunctionalityFlags::UPDATABLE; }

	PathController(physics::DynamicBody *body);
	virtual ~PathController();

	void addVertex(Vertex v);
	void addRedirect(int index);
	void start(float speed);

	void update(float dt);

private:
	PathLerper<Vertex> lerper_;
	physics::DynamicBody* body_;
};

#endif /* PHYSICS_PATHCONTROLLER_H_ */
