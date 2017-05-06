/*
 * SessionManager.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: bog
 */

#include "SessionManager.h"
#include "World.h"
#include "entities/Box.h"
#include "renderOpenGL/IViewportManager.h"
#include "renderOpenGL/Viewport.h"
#include "renderOpenGL/Camera.h"

World* SessionManager::wld_ = nullptr;
IViewportManager* SessionManager::vpm_ = nullptr;

void SessionManager::init(World& world, IViewportManager &vpm) {
	wld_ = &world;
	vpm_ = &vpm;
	// other init stuff here
}

void SessionManager::startSession(SessionTypes type) {
	resetSession();
	switch (type) {
	case TEST_SESSION:
		createTestSession();
		break;
	case TRANSFORM_SESSION:
		createTransformSession();
		break;
	case GAME_SESSION:
		createGameSession();
		break;
	}
}

void SessionManager::resetSession() {
	wld_->reset();
	vpm_->clearViewports();
	// TODO reset player
}

void SessionManager::createTestSession() {
	auto vp = std::make_unique<Viewport>(0, 0, vpm_->windowWidth(), vpm_->windowHeight());
	vp->camera()->moveTo({0, 0, -3});
	vpm_->addViewport("main", std::move(vp));
	wld_->takeOwnershipOf(std::make_unique<Box>(1, 1, 1, glm::vec3(-1.f, +1.f, 0.f)));
	wld_->takeOwnershipOf(std::make_unique<Box>(1, 1, 1, glm::vec3(+1.f, +1.f, 0.f)));
	wld_->takeOwnershipOf(std::make_unique<Box>(1, 1, 1, glm::vec3(-1.f, -1.f, 0.f)));
	wld_->takeOwnershipOf(std::make_unique<Box>(1, 1, 1, glm::vec3(+1.f, -1.f, 0.f)));
}

void SessionManager::createTransformSession() {

}

void SessionManager::createGameSession() {

}

