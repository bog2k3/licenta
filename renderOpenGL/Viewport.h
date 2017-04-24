#pragma once

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

class Camera;

class Viewport
{
public:
	Viewport(int x, int y, int w, int h);
	virtual ~Viewport();

	glm::vec4 getBkColor() const { return glm::vec4(0); }
	Camera* getCamera() const { return pCamera; }
	int getWidth() const { return viewportArea.z; }
	int getHeight() const { return viewportArea.w; }
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
};
