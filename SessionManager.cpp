/*
 * SessionManager.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: bog
 */

#include "SessionManager.h"
#include "World.h"
#include "entities/Box.h"

World* SessionManager::wld_ = nullptr;

void SessionManager::init(World& world) {
	wld_ = &world;
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
	// TODO reset player
}

void SessionManager::createTestSession() {
	wld_->takeOwnershipOf(std::make_unique<Box>(1, 1, 1, glm::vec3(-1.f, +1.f, 0.f)));
	wld_->takeOwnershipOf(std::make_unique<Box>(1, 1, 1, glm::vec3(+1.f, +1.f, 0.f)));
	wld_->takeOwnershipOf(std::make_unique<Box>(1, 1, 1, glm::vec3(-1.f, -1.f, 0.f)));
	wld_->takeOwnershipOf(std::make_unique<Box>(1, 1, 1, glm::vec3(+1.f, -1.f, 0.f)));
}

void SessionManager::createTransformSession() {

}

void SessionManager::createGameSession() {

}

