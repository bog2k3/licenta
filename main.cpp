#include <iostream>

#include "renderOpenGL/glToolkit.h"
#include "renderOpenGL/Shape2D.h"
#include "renderOpenGL/Renderer.h"
#include "renderOpenGL/Viewport.h"
#include "renderOpenGL/GLText.h"
#include "renderOpenGL/MeshRenderer.h"
#include "renderOpenGL/Camera.h"
#include "input/GLFWInput.h"
#include "input/InputEvent.h"
#include "World.h"
#include "math/math3D.h"
#include "OSD/SignalViewer.h"
#include "OSD/EntityLabeler.h"
#include "Infrastructure.h"
#include "SessionManager.h"

#include "utils/log.h"
#include "utils/DrawList.h"
#include "utils/UpdateList.h"
#include "utils/rand.h"
#include "utils/bitFlags.h"

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include <sstream>
#include <iomanip>
#include <functional>
#include <stdexcept>
#include <cstdio>

#include <sys/stat.h>

#ifdef DEBUG_DMALLOC
#include <dmalloc.h>
#endif

bool updatePaused = false;
bool slowMo = false;

template<> void update(std::function<void(float)> *fn, float dt) {
	(*fn)(dt);
}

void onInputEventHandler(InputEvent& ev) {
	if (ev.isConsumed())
		return;
	if (ev.key == GLFW_KEY_SPACE) {
		if (ev.type == InputEvent::EV_KEY_DOWN)
			updatePaused ^= true;
	} else if (ev.key == GLFW_KEY_S) {
		if (ev.type == InputEvent::EV_KEY_DOWN)
			slowMo ^= true;
	}
}

int main(int argc, char* argv[]) {
	do {
	#ifdef DEBUG
		updatePaused = true;
	#endif

		int windowW = 1024;
		int windowH = 720;

		// initialize stuff:
		if (!gltInit(windowW, windowH, "Transformari 3D - Ionita Bogdan"))
			return -1;

		GLFWInput::initialize(gltGetWindow());
		GLFWInput::onInputEvent.add(onInputEventHandler);

		Renderer renderer(windowW, windowH);
		World world;

		randSeed(time(NULL));

		DrawList drawList;
		drawList.add(World::getInstance());
		drawList.add(&EntityLabeler::getInstance());

		UpdateList continuousUpdateList;

		UpdateList updateList;
		updateList.add(World::getInstance());

		SessionManager::init(world, renderer);
		SessionManager::startSession(SessionManager::TRANSFORM_SESSION);

		// initial update:
		updateList.update(0);

		float t = glfwGetTime();
		while (GLFWInput::checkInput()) {
			float newTime = glfwGetTime();
			float frameTime = newTime - t;
			t = newTime;

			// fixed time step for simulation (unless slowMo is on)
			float simDT = updatePaused ? 0 : std::min(frameTime, 1.f / 20); // don't let the dt increase below 20 FPS
			if (slowMo) {
				simDT *= 0.1f;
			}

			continuousUpdateList.update(frameTime);
			if (simDT > 0) {
				updateList.update(simDT);
			}

			// wait until previous frame finishes rendering and show frame output:
			gltEnd();
			// build the render queue for the current frame
			drawList.draw();

//			GLText::get()->print("Transformari 3D - Ionita Bogdan Florin",
//					{20, 20, ViewportCoord::absolute, ViewportCoord::bottom | ViewportCoord::left},
//					0, 16, glm::vec3(0.2f, 0.4, 1.0f));

			if (updatePaused) {
				GLText::get()->print("PAUSED", {45, 50, ViewportCoord::percent}, 0, 32, glm::vec3(1.f, 0.8f, 0.2f));
			}
//			if (slowMo) {
//				GLText::get()->print("~~ Slow Motion ON ~~", {40, 5, ViewportCoord::percent}, 0, 20, glm::vec3(1.f, 0.5f, 0.1f));
//			}

			// do the actual openGL render for the previous frame (which is independent of our world)
			gltBegin();
			renderer.render();
			// now rendering is on-going, move on to the next update:
		}

		renderer.unload();
		Infrastructure::shutDown();
	} while (0);

	std::cout << "\n\n";

	return 0;
}
