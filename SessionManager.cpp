/*
 * SessionManager.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: bog
 */

#include "SessionManager.h"
#include "World.h"
#include "entities/Box.h"
#include "entities/CameraController.h"
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
	// set up viewports
	int hw = vpm_->windowWidth()/2;
	int hh = vpm_->windowHeight()/2;
	// viewport 1
	auto vp = std::make_unique<Viewport>(0, hh-5, hw-5, hh-5);
	vp->camera()->moveTo({0, 0, -3});
	vpm_->addViewport("translation", std::move(vp));
	// viewport 2
	vp = std::make_unique<Viewport>(hw+5, hh-5, hw-5, hh-5);
	vp->camera()->moveTo({1, 0, -3});
	vp->camera()->lookAt({0, 0, 0});
	vpm_->addViewport("rotation", std::move(vp));
	// viewport 3
	vp = std::make_unique<Viewport>(0, 0, hw-5, hh-5);
	vp->camera()->moveTo({-3, 0, -1});
	vp->camera()->lookAt({0, 0, 0});
	vpm_->addViewport("scale", std::move(vp));
	// viewport 4
	vp = std::make_unique<Viewport>(hw+5, 0, hw-5, hh-5);
	vp->camera()->moveTo({0, 2, +3});
	vp->camera()->lookAt({0, 0, 0});
	vpm_->addViewport("combined", std::move(vp));

	// create entities
	wld_->takeOwnershipOf(std::make_unique<Box>(1, 1, 1, glm::vec3(-1.f, +1.f, 0.f)));
	wld_->takeOwnershipOf(std::make_unique<Box>(1, 1, 1, glm::vec3(+1.f, +1.f, 0.f)));
	wld_->takeOwnershipOf(std::make_unique<Box>(1, 1, 1, glm::vec3(-1.f, -1.f, 0.f)));
	wld_->takeOwnershipOf(std::make_unique<Box>(1, 1, 1, glm::vec3(+1.f, -1.f, 0.f)));

	auto camCtrl = std::make_unique<CameraController>(vpm_->getViewport("rotation")->camera());
	camCtrl->startBackAndForth({-3, 0, -3}, {3, 0, -3}, {0, 0, 0}, 2.f);
	camCtrl->startPath({
		{CameraController::pathNode::vertex, {-3, 0, -3}, {-3, 0, -3}, 0},	//#0
		{CameraController::pathNode::vertex, {3, 0, 3}, {3, 0, 3}, 0},	//#1
		{CameraController::pathNode::redirect, {0,0,0}, {0,0,0}, 0}	// -> #0
	}, 2.f);
	wld_->takeOwnershipOf(std::move(camCtrl));
}

void SessionManager::createGameSession() {

}

