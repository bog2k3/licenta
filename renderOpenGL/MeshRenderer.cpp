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
	glEnableVertexAttribArray(indexPos_);
	glEnableVertexAttribArray(indexNorm_);
	glEnableVertexAttribArray(indexUV1_);
	glEnableVertexAttribArray(indexColor_);

	auto matVP = vp->getCamera()->getMatViewProj();

	for (auto &m : renderQueue_) {
		auto matWVP = matVP * m.wldTransform_;
		glUniformMatrix4fv(indexMatViewProj_, 1, GL_FALSE, glm::value_ptr(matWVP));

		glBindBuffer(GL_ARRAY_BUFFER, m.pMesh_->getVertexBuffer());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.pMesh_->getIndexBuffer());
		glVertexAttribPointer(indexPos_, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::s_Vertex), (void*)offsetof(Mesh::s_Vertex, position));
		glVertexAttribPointer(indexNorm_, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::s_Vertex), (void*)offsetof(Mesh::s_Vertex, normal));
		glVertexAttribPointer(indexUV1_, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::s_Vertex), (void*)offsetof(Mesh::s_Vertex, UV1));
		glVertexAttribPointer(indexColor_, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::s_Vertex), (void*)offsetof(Mesh::s_Vertex, color));

		glDrawElements(GL_TRIANGLES, m.pMesh_->getIndexCount(), GL_UNSIGNED_SHORT, 0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshRenderer::purgeRenderQueue() {
	renderQueue_.clear();
}
