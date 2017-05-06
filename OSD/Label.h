#pragma once

#include "../renderOpenGL/ViewportCoord.h"

#include <string>
#include <glm/vec3.hpp>

class Label
{
public:

	Label(std::string value, ViewportCoord pos, float z, float textSize, glm::vec3 color);

	void setText(std::string text) { value_ = text; }
	void setColor(glm::vec3 rgb) { color_ = rgb; }
	void setTextSize(float size) { textSize_ = size; }
	void setPos(ViewportCoord pos) { pos_ = pos; }

	glm::vec2 boxSize() const;

	void draw();

	bool drawFrame_ = true;

protected:
	ViewportCoord pos_;
	float z_;
	glm::vec3 color_;
	float textSize_;
	std::string value_;
};
