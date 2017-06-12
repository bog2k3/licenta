/*
 * LabelEntity.cpp
 *
 *  Created on: May 18, 2017
 *      Author: bog
 */

#include "SigViewerEntity.h"

SigViewerEntity::SigViewerEntity(ViewportCoord pos, float z, ViewportCoord size, std::set<std::string> viewportFilter)
	: sigViewer_(std::make_unique<SignalViewer>(pos, z, size, viewportFilter))
{
}

SigViewerEntity::~SigViewerEntity() {
}

void SigViewerEntity::draw() {
	sigViewer_->draw();
}

