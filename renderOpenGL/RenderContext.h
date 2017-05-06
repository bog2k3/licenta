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
	RenderLayers enabledLayers;

	RenderContext()
		: viewport(nullptr) {
	}

	RenderContext(Viewport* vp)
		: viewport(vp) {
	}
};

#endif /* RENDEROPENGL_RENDERCONTEXT_H_ */
