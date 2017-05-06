/*
 * SessionManager.h
 *
 *  Created on: Apr 24, 2017
 *      Author: bog
 */

#ifndef SESSIONMANAGER_H_
#define SESSIONMANAGER_H_

class World;
class IViewportManager;

class SessionManager {
public:
	static void init(World& world, IViewportManager& vpm);

	enum SessionTypes {
		TEST_SESSION,
		TRANSFORM_SESSION,
		GAME_SESSION,
	};

	static void startSession(SessionTypes type);

private:
	static World* wld_;
	static IViewportManager* vpm_;

	static void resetSession();
	static void createTestSession();
	static void createTransformSession();
	static void createGameSession();
};

#endif /* SESSIONMANAGER_H_ */
