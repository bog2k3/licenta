/*
 * Mesh.h
 *
 *  Created on: Apr 24, 2017
 *      Author: bog
 */

#ifndef RENDEROPENGL_MESH_H_
#define RENDEROPENGL_MESH_H_

// TODO create a MeshRenderer: IRenderable, put in renderContext, it will manage all meshes
class Mesh {
public:
	Mesh();
	virtual ~Mesh();

	void createBox(float width, float height, float depth);
	void createSphere(float radius, int detail=10);
	void setCenter(glm::vec3 center);
};

#endif /* RENDEROPENGL_MESH_H_ */
