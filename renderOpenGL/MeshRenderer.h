/*
 * MeshRenderer.h
 *
 *  Created on: Apr 25, 2017
 *      Author: bog
 */

#ifndef RENDEROPENGL_MESHRENDERER_H_
#define RENDEROPENGL_MESHRENDERER_H_

#include "IRenderable.h"

#include <glm/mat4x4.hpp>
#include <vector>

class Mesh;
class Renderer;

class MeshRenderer: public IRenderable {
public:
	MeshRenderer(Renderer* renderer);
	virtual ~MeshRenderer();

	void renderMesh(Mesh& mesh, glm::mat4 worldTransform = glm::mat4(1));

private:
	void render(Viewport* pCrtViewport) override;
	void purgeRenderQueue() override;

	struct meshRenderData {
		Mesh* pMesh_;
		glm::mat4 wldTransform_;

		meshRenderData(Mesh* pMesh, glm::mat4 wldTransform)
			: pMesh_(pMesh), wldTransform_(wldTransform) {
		}
		meshRenderData(meshRenderData &&) = default;
	};
	std::vector<meshRenderData> renderQueue_;
	unsigned meshShaderProgram_ = 0;
	unsigned indexPos_ = 0;
	unsigned indexNorm_ = 0;
	unsigned indexUV1_ = 0;
	unsigned indexColor_ = 0;
	unsigned indexMatViewProj_ = 0;
};

#endif /* RENDEROPENGL_MESHRENDERER_H_ */
