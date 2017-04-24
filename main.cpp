#include <iostream>

#include "renderOpenGL/glToolkit.h"
#include "renderOpenGL/Shape2D.h"
#include "renderOpenGL/Renderer.h"
#include "renderOpenGL/Viewport.h"
#include "renderOpenGL/GLText.h"
#include "input/GLFWInput.h"
#include "input/InputEvent.h"
#include "World.h"
#include "math/math3D.h"
#include "OSD/SignalViewer.h"
#include "OSD/EntityLabeler.h"
#include "Infrastructure.h"

#include "utils/log.h"
#include "utils/DrawList.h"
#include "utils/UpdateList.h"
#include "utils/rand.h"

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

#define FFMT(prec, X) std::fixed << std::setprecision(prec) << (X)
#define IFMT(spac, X) std::fixed << std::setprecision(0) << std::setw(spac) << (X)

void printStatus(float simulationTime, float realTime, float simDTAcc, float realDTAcc) {
	LOGLN(	"SIM-TIME: " << IFMT(5, simulationTime)
			<< "\tREAL-time: "<< IFMT(5, realTime)
			<< "\tINST-MUL: " << FFMT(2, simDTAcc/realDTAcc)
			<< "\tAVG-MUL: " << FFMT(2, simulationTime/realTime)
		);
}

int main(int argc, char* argv[]) {
	do {
	#ifdef DEBUG
		updatePaused = true;
		skipRendering = false;
	#endif

		int windowW = 1024;
		int windowH = 768;

		// initialize stuff:
		if (!gltInit(windowW, windowH, "Transformari 3D - Ionita Bogdan"))
			return -1;

		GLFWInput::initialize(gltGetWindow());
		GLFWInput::onInputEvent.add(onInputEventHandler);

		Renderer renderer;
		Viewport vp1(0, 0, windowW, windowH);
		renderer.addViewport(&vp1);
		auto shape2d = new Shape2D(&renderer);
		auto gltext = new GLText(&renderer, "data/fonts/DejaVuSansMono_256_16_8.png", 8, 16, ' ', 22);
		RenderContext renderContext( &vp1, shape2d, gltext);

		World world;

		//randSeed(1424118659);
		randSeed(time(NULL));
		LOGLN("RAND seed: "<<rand_seed);

		SignalViewer sigViewer(glm::vec3(0.75f, 0.1f, 1.f), glm::vec2(0.2f, 0.1f));

		DrawList drawList;
		drawList.add(World::getInstance());
		drawList.add(&sigViewer);
		drawList.add(&EntityLabeler::getInstance());

		UpdateList continuousUpdateList;

		UpdateList updateList;
		updateList.add(World::getInstance());
		updateList.add(&sigViewer);

		float realTime = 0;							// [s]
		float simulationTime = 0;					// [s]
		float lastPrintedSimTime = 0;				// [s]
		float simDTAcc = 0; // [s] accumulated sim dt values since last status print
		float realDTAcc = 0; // [s] accumulated real dt values since last status print
		constexpr float simTimePrintInterval = 10.f; // [s]

		float frameTime = 0;

		sigViewer.addSignal("frameTime", &frameTime,
				glm::vec3(1.f, 0.2f, 0.2f), 0.1f);

		// initial update:
		updateList.update(0);

		float t = glfwGetTime();
		while (GLFWInput::checkInput()) {
			float newTime = glfwGetTime();
			float realDT = newTime - t;
			frameTime = realDT;
			realDTAcc += realDT;
			t = newTime;
			realTime += realDT;

			// fixed time step for simulation (unless slowMo is on)
			float simDT = updatePaused ? 0 : 0.02f;
			if (slowMo) {
				// use same fixed timestep in order to avoid breaking physics, but
				// only update once every n frames to slow down
				static float frameCounter = 0;
				constexpr float cycleLength = 10; // frames
				if (++frameCounter == cycleLength) {
					frameCounter = 0;
				} else
					simDT = 0;
			}

			simulationTime += simDT;
			simDTAcc += simDT;

			if (simulationTime > lastPrintedSimTime+simTimePrintInterval) {
				printStatus(simulationTime, realTime, simDTAcc, realDTAcc);
				simDTAcc = realDTAcc = 0;
				lastPrintedSimTime = simulationTime;
			}

			continuousUpdateList.update(realDT);
			if (simDT > 0) {
				updateList.update(simDT);
			}

			// wait until previous frame finishes rendering and show frame output:
			gltEnd();
			// draw builds the render queue for the current frame
			drawList.draw(renderContext);

			renderContext.text->print("Transformari 3D - Ionita Bogdan Florin", 20, vp1.getHeight()-20, 0, 16, glm::vec3(0.2f, 0.4, 1.0f));

			if (updatePaused) {
				renderContext.text->print("PAUSED", vp1.getWidth() / 2, vp1.getHeight() / 2, 0, 32, glm::vec3(1.f, 0.8f, 0.2f));
			}
			if (slowMo) {
				renderContext.text->print("~~ Slow Motion ON ~~", 10, 45, 0, 18, glm::vec3(1.f, 0.5f, 0.1f));
			}

			// do the actual openGL render for the previous frame (which is independent of our world)
			gltBegin();
			renderer.render();
			// now rendering is on-going, move on to the next update:
		}

		delete renderContext.shape;
		Infrastructure::shutDown();
	} while (0);

	std::cout << "\n\n";

	return 0;
}
