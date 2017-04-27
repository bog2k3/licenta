/*
 * DynamicBody.h
 *
 *  Created on: Apr 27, 2017
 *      Author: bog
 */

#ifndef PHYSICS_DYNAMICBODY_H_
#define PHYSICS_DYNAMICBODY_H_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace physics {

class DynamicBody {
public:
	DynamicBody();
	virtual ~DynamicBody();

	void update(float dt);
};

} /* namespace physics */

#endif /* PHYSICS_DYNAMICBODY_H_ */
