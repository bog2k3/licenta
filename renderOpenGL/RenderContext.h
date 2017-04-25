/*
 * RenderContext.h
 *
 *  Created on: Dec 23, 2014
 *      Author: bogdan
 */

#ifndef RENDEROPENGL_RENDERCONTEXT_H_
#define RENDEROPENGL_RENDERCONTEXT_H_

class Shape2D;
class Viewport;
class GLText;
class MeshRenderer;

struct RenderLayers {
	bool meshes = true;	// 3D meshes
	bool osd = true;	// 2D on-screen-display
};

class RenderContext {
public:
	const Viewport* const viewport;
	Shape2D* const shape;
	MeshRenderer* const meshRenderer;
	GLText* const text;

	RenderLayers enabledLayers;

	RenderContext()
		: viewport(nullptr)
		, shape(nullptr)
		, meshRenderer(nullptr)
		, text(nullptr) {
	}

	RenderContext(Viewport* vp, Shape2D* shape, MeshRenderer* mesher, GLText* text)
		: viewport(vp), shape(shape), meshRenderer(mesher), text(text) {
	}
};

#endif /* RENDEROPENGL_RENDERCONTEXT_H_ */
