/*
 * MeshRenderer.cpp
 *
 *  Created on: Apr 25, 2017
 *      Author: bog
 */

#include "MeshRenderer.h"
#include "Renderer.h"
#include "Viewport.h"
#include "Mesh.h"

MeshRenderer::MeshRenderer(Renderer* renderer) {
	renderer->registerRenderable(this);
}

MeshRenderer::~MeshRenderer() {
	// TODO Auto-generated destructor stub
}

void MeshRenderer::renderMesh(Mesh& mesh, glm::mat4 worldTransform) {
	renderQueue_.push_back(meshRenderData(&mesh, worldTransform));
}

void MeshRenderer::render(Viewport* pCrtViewport) {
	for (auto &m : renderQueue_) {
		// do stuff
	}
}

void MeshRenderer::purgeRenderQueue() {
	renderQueue_.clear();
}
