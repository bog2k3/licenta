/*
 * Renderer.cpp
 *
 *  Created on: Nov 2, 2014
 *      Author: bog
 */

#include "Renderer.h"
#include "Viewport.h"
#include "Shape2D.h"
#include "GLText.h"
#include "MeshRenderer.h"
#include "../utils/DrawList.h"
#include <stdexcept>

Renderer::~Renderer() {
}

Renderer::Renderer() {
	Shape2D::init(this);
	GLText::init(this, "data/fonts/DejaVuSansMono_256_16_8.png", 8, 16, ' ', 22);
	MeshRenderer::init(this);
}

void Renderer::registerRenderable(IRenderable* r) {
	if (r == nullptr)
		throw new std::invalid_argument("argument cannot be null!");
	renderComponents_.push_back(r);
}

void Renderer::addViewport(std::string name, Viewport &vp) {
	viewports_[name] = &vp;
}

void Renderer::render() {
	for (auto r : renderComponents_) {
		for (auto &vp : viewports_) {
			if (!vp.second->isEnabled())
				continue;
			r->render(vp.second);
		}
		r->purgeRenderQueue();
	}
}
