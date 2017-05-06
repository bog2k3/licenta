#pragma once

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class Camera;

class Viewport
{
public:
	Viewport(int x, int y, int w, int h);
	virtual ~Viewport();

	glm::vec3 getBkColor() const { return backgroundColor_; }
	void setBkColor(glm::vec3 c) { backgroundColor_ = c; }
	Camera* getCamera() const { return pCamera; }
	int width() const { return viewportArea.z; }
	int height() const { return viewportArea.w; }
	glm::vec2 getPosition() const { return glm::vec2(viewportArea.x, viewportArea.y); }
	bool isEnabled() const { return mEnabled; }
	bool containsPoint(glm::vec2 const&p) const;
	/**
	 * returned vector: x-X, y-Y, z-Width, w-Height
	 */
	glm::vec4 getScreenRect() const {return viewportArea; }
	glm::vec3 project(glm::vec3 point) const;
	glm::vec3 unproject(glm::vec3 point) const;

	void setEnabled(bool enabled) { mEnabled = enabled; }
	void setArea(int vpX, int vpY, int vpW, int vpH);

	long getUserData() { return m_userData; }
	void setUserData(long data) { m_userData = data; }

protected:
	long m_userData;
	glm::vec4 viewportArea;
	Camera* pCamera;
	bool mEnabled;
	glm::vec3 backgroundColor_;
};
