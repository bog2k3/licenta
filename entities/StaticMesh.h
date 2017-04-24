/*
 * EStaticMesh.h
 *
 *  Created on: Apr 9, 2017
 *      Author: bog
 */

#ifndef ENTITIES_STATICMESH_H_
#define ENTITIES_STATICMESH_H_

#include "Entity.h"

class StaticMesh: public Entity {
public:
	int getEntityType() const override { return EntTypes::STATICMESH; }
	StaticMesh();
	virtual ~StaticMesh();
};

#endif /* ENTITIES_STATICMESH_H_ */
