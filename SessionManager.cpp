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
#include "entities/LabelEntity.h"
#include "entities/PathController.h"
#include "entities/SigViewerEntity.h"
#include "renderOpenGL/IViewportManager.h"
#include "renderOpenGL/Viewport.h"
#include "renderOpenGL/ViewportCoord.h"
#include "renderOpenGL/Camera.h"

#include <glm/gtc/quaternion.hpp>

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
	auto vp = std::make_unique<Viewport>(0, hh+5, hw-5, hh-5);
	vp->camera()->moveTo({-1, 1, 0});
	vp->camera()->lookAt({-2, 0.5f, 0});
	vp->setBkColor({0.1f, 0.1f, 0.1f});
	vpm_->addViewport("translation", std::move(vp));
	// viewport 2
	vp = std::make_unique<Viewport>(hw+5, hh+5, hw-5, hh-5);
	vp->camera()->moveTo({0, 1, 1});
	vp->camera()->lookAt({0, 0.5f, 2});
	vp->setBkColor({0.1f, 0.1f, 0.1f});
	vpm_->addViewport("rotation", std::move(vp));
	// viewport 3
	vp = std::make_unique<Viewport>(0, 0, hw-5, hh-5);
	vp->camera()->moveTo({1, 1, 0});
	vp->camera()->lookAt({2, 0.5f, 0});
	vp->setBkColor({0.1f, 0.1f, 0.1f});
	vpm_->addViewport("scale", std::move(vp));
	// viewport 4
	vp = std::make_unique<Viewport>(hw+5, 0, hw-5, hh-5);
	vp->camera()->moveTo({0, 3, -4});
	vp->camera()->lookAt({0, 0.5f, -7});
	vp->setBkColor({0.1f, 0.1f, 0.1f});
	vpm_->addViewport("combined", std::move(vp));

	// create entities
	auto box1 = std::make_unique<Box>(1, 1, 1);
	auto pc = std::make_unique<PathController>(box1->body());
	wld_->takeOwnershipOf(std::move(box1));
	pc->addVertex({{-3, 0, -1}, glm::fquat()});
	pc->addVertex({{-3, 0, +1}, glm::fquat()});
	pc->addRedirect(0);
	pc->start(1.5f);
	auto sv1 = std::make_unique<SigViewerEntity>(
			ViewportCoord(27, 5, ViewportCoord::percent, ViewportCoord::top | ViewportCoord::right), 1.f,
			ViewportCoord(25, 15, ViewportCoord::percent), std::set<std::string>{"translation"});
	auto pcp1 = pc.get();
	wld_->takeOwnershipOf(std::move(pc));
	sv1->get().addSignal("translatie",
			[pcp1]() -> float { return (pcp1->value().position - pcp1->vertex(0).position).z; },
			glm::vec3(1.f, 0.2f, 0.2f), 0.05f, 50, 1.5f, -1.5f, 2);
	wld_->takeOwnershipOf(std::move(sv1));

	auto box2 = std::make_unique<Box>(1, 1, 1);
	pc = std::make_unique<PathController>(box2->body());
	wld_->takeOwnershipOf(std::move(box2));
	pc->addVertex({glm::vec3{0.f, 0.f, 3.f}, glm::angleAxis(0.f, glm::vec3{0, 1, 0})});
	pc->addVertex({glm::vec3{0.f, 0.f, 3.f}, glm::angleAxis(PI/1.5f, glm::vec3{0, 1, 0})});
	pc->addVertex({glm::vec3{0.f, 0.f, 3.f}, glm::angleAxis(2*PI/1.5f, glm::vec3{0, 1, 0})});
	pc->addRedirect(0);
	pc->start(2.5f);
	auto sv2 = std::make_unique<SigViewerEntity>(
			ViewportCoord(27, 5, ViewportCoord::percent, ViewportCoord::top | ViewportCoord::right), 1.f,
			ViewportCoord(25, 15, ViewportCoord::percent), std::set<std::string>{"rotation"});
	auto pcp2 = pc.get();
	wld_->takeOwnershipOf(std::move(pc));
	sv2->get().addSignal("rotatie",
			[pcp2]() -> float { return (pcp2->value() - pcp2->vertex(0)) * 180 / PI; },
			glm::vec3(1.f, 0.2f, 0.2f), 0.05f, 50, 360, 0, 0);
	wld_->takeOwnershipOf(std::move(sv2));

	auto box3 = std::make_unique<Box>(1, 1, 1);
	pc = std::make_unique<PathController>(box3->body());
	wld_->takeOwnershipOf(std::move(box3));
	pc->addVertex({glm::vec3{3, 0, 0}, glm::fquat{}, glm::vec3{1, 0.5f, 1}});
	pc->addVertex({glm::vec3{3, 0, 0}, glm::fquat{}, glm::vec3{1, 2.5f, 1}});
	pc->addRedirect(0);
	pc->start(2.2f);
	auto sv3 = std::make_unique<SigViewerEntity>(
			ViewportCoord(27, 5, ViewportCoord::percent, ViewportCoord::top | ViewportCoord::right), 1.f,
			ViewportCoord(25, 15, ViewportCoord::percent), std::set<std::string>{"scale"});
	auto pcp3 = pc.get();
	wld_->takeOwnershipOf(std::move(pc));
	sv3->get().addSignal("scalare",
			[pcp3]() -> float { return pcp3->value().scale.y; },
			glm::vec3(1.f, 0.2f, 0.2f), 0.05f, 50, 3, 0, 2);
	wld_->takeOwnershipOf(std::move(sv3));

	auto box4 = std::make_unique<Box>(1, 1, 1);
	auto box5 = std::make_unique<Box>(0.5f, 0.5f, 0.5f);
	box5->body()->setParent(box4->body());	// attach box5 to box4

	pc = std::make_unique<PathController>(box5->body());
	wld_->takeOwnershipOf(std::move(box5));
	pc->addVertex({glm::vec3{-1.f, 0.5f, -1.f}, glm::fquat{}});
	pc->addVertex({glm::vec3{-1.f, 0.5f, -1.f}, glm::angleAxis(-160.f, glm::vec3{0, 1, 0})});
	pc->addVertex({glm::vec3{-1.f, 0.5f, -1.f}, glm::angleAxis(-240.f, glm::vec3{0, 1, 0})});
	pc->addRedirect(0);
	pc->start(2.4f);
	wld_->takeOwnershipOf(std::move(pc));

	pc = std::make_unique<PathController>(box4->body());
	wld_->takeOwnershipOf(std::move(box4));
	pc->addVertex({glm::vec3{0, 0, -8.f}, glm::fquat{}, glm::vec3{0.5f, 1, 1}});
	pc->addVertex({glm::vec3{0, 0, -8.f}, glm::angleAxis(180.f, glm::vec3{0, 1, 0}), glm::vec3{2.5f, 1, 1}});
	pc->addRedirect(0);
	pc->start(1.8f);
	wld_->takeOwnershipOf(std::move(pc));

	wld_->takeOwnershipOf(std::make_unique<LabelEntity>("Translatii", 22,
			ViewportCoord{10, 10, ViewportCoord::absolute, ViewportCoord::left | ViewportCoord::bottom}, 0,
			glm::vec3{1, 1, 1}, std::set<std::string>{"translation"}));
	wld_->takeOwnershipOf(std::make_unique<LabelEntity>("Rotatii", 22,
			ViewportCoord{10, 10, ViewportCoord::absolute, ViewportCoord::left | ViewportCoord::bottom}, 0,
			glm::vec3{1, 1, 1}, std::set<std::string>{"rotation"}));
	wld_->takeOwnershipOf(std::make_unique<LabelEntity>("Scalari", 22,
			ViewportCoord{10, 10, ViewportCoord::absolute, ViewportCoord::left | ViewportCoord::bottom}, 0,
			glm::vec3{1, 1, 1}, std::set<std::string>{"scale"}));
	wld_->takeOwnershipOf(std::make_unique<LabelEntity>("Combinate", 22,
			ViewportCoord{10, 10, ViewportCoord::absolute, ViewportCoord::left | ViewportCoord::bottom}, 0,
			glm::vec3{1, 1, 1}, std::set<std::string>{"combined"}));

	//auto camCtrl = std::make_unique<CameraController>(vpm_->getViewport("rotation")->camera());
	//camCtrl->startBackAndForth({-3, 0, -3}, {3, 0, -3}, {0, 0, 0}, 2.f);
	//wld_->takeOwnershipOf(std::move(camCtrl));
}

void SessionManager::createGameSession() {

}

