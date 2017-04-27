/*
 * Cube.h
 *
 *  Created on: Apr 24, 2017
 *      Author: bog
 */

#ifndef ENTITIES_CUBE_H_
#define ENTITIES_CUBE_H_

#include "Entity.h"
#include "../renderOpenGL/Mesh.h"

class Cube: public Entity {
public:
	Cube(float width, float height, float depth, glm::vec3 offset = glm::vec3(0));
	virtual ~Cube();

	int getEntityType() const override { return EntTypes::CUBE; }
	FunctionalityFlags getFunctionalityFlags() const override { return FunctionalityFlags::DRAWABLE | FunctionalityFlags::UPDATABLE; }

	void update(float dt) override;
	void draw(RenderContext const& ctx) override;

private:
	Mesh mesh_;
};

#endif /* ENTITIES_CUBE_H_ */
