/*
 * Cube.h
 *
 *  Created on: Apr 24, 2017
 *      Author: bog
 */

#ifndef ENTITIES_CUBE_H_
#define ENTITIES_CUBE_H_

#include "Entity.h"

class Cube: public Entity {
public:
	Cube();
	virtual ~Cube();

	int getEntityType() const override { return EntTypes::CUBE; }
	FunctionalityFlags getFunctionalityFlags() const override { return FunctionalityFlags::DRAWABLE | FunctionalityFlags::UPDATABLE; }

	void update(float dt) override;
	void draw(RenderContext const& ctx) override;
};

#endif /* ENTITIES_CUBE_H_ */
