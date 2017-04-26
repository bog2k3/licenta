/*
 * Mesh.h
 *
 *  Created on: Apr 24, 2017
 *      Author: bog
 */

#ifndef RENDEROPENGL_MESH_H_
#define RENDEROPENGL_MESH_H_

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Mesh {
public:
	Mesh();
	virtual ~Mesh();

	void createBox(float width, float height, float depth);
	void createSphere(float radius, int detail=10);
	void setCenter(glm::vec3 center);

	unsigned getVertexBuffer() const { return vertexBuffer_; }
	unsigned getIndexBuffer() const { return indexBuffer_; }
	unsigned getVertexCount() const { return vertexCount_; }
	unsigned getIndexCount() const { return indexCount_; }

private:
	friend class MeshRenderer;

	unsigned vertexBuffer_ = 0;
	unsigned indexBuffer_ = 0;
	unsigned vertexCount_ = 0;
	unsigned indexCount_ = 0;

	struct s_Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 UV1;
		glm::vec3 color;
	};
};

#endif /* RENDEROPENGL_MESH_H_ */
