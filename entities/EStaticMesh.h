/*
 * EStaticMesh.h
 *
 *  Created on: Apr 9, 2017
 *      Author: bog
 */

#ifndef ENTITIES_ESTATICMESH_H_
#define ENTITIES_ESTATICMESH_H_

#include "Entity.h"

class EStaticMesh: public Entity {
public:
	int getEntityType() const override { return EntTypes::E_STATICMESH; }
	EStaticMesh();
	virtual ~EStaticMesh();
};

#endif /* ENTITIES_ESTATICMESH_H_ */
