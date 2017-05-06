/*
 * Renderer.h
 *
 *  Created on: Nov 2, 2014
 *      Author: bog
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include "IRenderable.h"
#include <vector>
#include <map>

class Renderer {
public:
	virtual ~Renderer();
	Renderer();

	void registerRenderable(IRenderable* r);

	void addViewport(std::string name, Viewport &vp);
	Viewport* getViewport(std::string name) const;
	std::vector<Viewport*> getViewports() const;
	void deleteViewport(std::string const& name);
	void clearViewports() { viewports_.clear(); }

	void render();

	void unload();

protected:
	std::vector<IRenderable*> renderComponents_;
	std::map<std::string, Viewport*> viewports_;
};

#endif /* RENDERER_H_ */
