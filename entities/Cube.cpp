/*
 * Cube.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: bog
 */

#include "Cube.h"

Cube::Cube(float width, float height, float depth, glm::vec3 offset) {
	mesh_.createBox(offset, width, height, depth);
}

Cube::~Cube() {
}

void Cube::update(float dt) {
	// TODO
}

void Cube::draw(RenderContext const& ctx) {
	ctx.meshRenderer->renderMesh(mesh_, glm::mat4(1));
}
