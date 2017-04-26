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
#include "shader.hpp"
#include "Camera.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/gtc/type_ptr.hpp>

MeshRenderer::MeshRenderer(Renderer* renderer) {
	renderer->registerRenderable(this);
	meshShaderProgram_ = Shaders::createProgram("data/shaders/mesh.vert", "data/shaders/mesh-texture.frag");
	if (meshShaderProgram_ == 0) {
		throw std::runtime_error("Unable to load mesh shaders!!");
	}
	indexPos_ = glGetAttribLocation(meshShaderProgram_, "vPos");
	indexNorm_ = glGetAttribLocation(meshShaderProgram_, "vNormal");
	indexUV1_ = glGetAttribLocation(meshShaderProgram_, "vUV1");
	indexColor_ = glGetAttribLocation(meshShaderProgram_, "vColor");
	indexMatViewProj_ = glGetUniformLocation(meshShaderProgram_, "mViewProj");
}

MeshRenderer::~MeshRenderer() {
	glDeleteProgram(meshShaderProgram_);
}

void MeshRenderer::renderMesh(Mesh& mesh, glm::mat4 worldTransform) {
	renderQueue_.push_back(meshRenderData(&mesh, worldTransform));
}

void MeshRenderer::render(Viewport* vp) {
	glUseProgram(meshShaderProgram_);
	auto matVP = vp->getCamera()->getMatViewProj();
		/*glBindBuffer(GL_ARRAY_BUFFER, 0);

		glUniformMatrix4fv(indexMatViewProj, 1, GL_FALSE, glm::value_ptr(vp->getCamera()->getMatViewProj()));
		glEnableVertexAttribArray(indexPos);
		glEnableVertexAttribArray(indexColor);

		// render world-space triangle primitives:
		glVertexAttribPointer(indexPos, 3, GL_FLOAT, GL_FALSE, sizeof(s_lineVertex), &bufferTri[0].pos);
		glVertexAttribPointer(indexColor, 4, GL_FLOAT, GL_FALSE, sizeof(s_lineVertex), &bufferTri[0].rgba);
		glDrawElements(GL_TRIANGLES, indicesTri.size(), GL_UNSIGNED_SHORT, &indicesTri[0]);
		 */

	for (auto &m : renderQueue_) {
		auto matWVP = matVP * m.wldTransform_;
		glUniformMatrix4fv(indexMatViewProj_, 1, GL_FALSE, glm::value_ptr(matWVP));
	}
}

void MeshRenderer::purgeRenderQueue() {
	renderQueue_.clear();
}
