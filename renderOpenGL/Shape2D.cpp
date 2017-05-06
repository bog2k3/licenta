/*
 * Line.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: bogdan
 */

#include "Shape2D.h"

#include "Renderer.h"
#include "Viewport.h"
#include "Camera.h"
#include "shader.hpp"
#include "../math/math3D.h"

#include <stdexcept>
#include <glm/mat4x4.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLEW_NO_GLU
#include <GL/glew.h>

static Shape2D* instance = nullptr;

void Shape2D::init(Renderer* renderer) {
	instance = new Shape2D(renderer);
}

Shape2D* Shape2D::get() {
	assertDbg(instance && "must be initialized first!");
	return instance;
}

Shape2D::Shape2D(Renderer* renderer)
	: lineShaderProgram(0)
	, indexPos(0)
	, indexColor(0)
	, indexMatViewProj(0)
{
	renderer->registerRenderable(this);
	lineShaderProgram = Shaders::createProgram("data/shaders/shape2d.vert", "data/shaders/shape2d.frag");
	if (lineShaderProgram == 0) {
		throw std::runtime_error("Unable to load line shaders!!");
	}
	indexPos = glGetAttribLocation(lineShaderProgram, "vPos");
	indexColor = glGetAttribLocation(lineShaderProgram, "vColor");
	indexMatViewProj = glGetUniformLocation(lineShaderProgram, "mViewProj");
}

Shape2D::~Shape2D() {
	glDeleteProgram(lineShaderProgram);
}

void Shape2D::unload() {
	delete instance;
	instance = nullptr;
}

void Shape2D::render(Viewport* vp) {
	glUseProgram(lineShaderProgram);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendEquation(GL_BLEND_EQUATION_ALPHA);

	glUniformMatrix4fv(indexMatViewProj, 1, GL_FALSE, glm::value_ptr(vp->getCamera()->getMatProjView()));
	glEnableVertexAttribArray(indexPos);
	glEnableVertexAttribArray(indexColor);

	// render world-space triangle primitives:
	glVertexAttribPointer(indexPos, 3, GL_FLOAT, GL_FALSE, sizeof(s_lineVertex), &bufferTri[0].pos);
	glVertexAttribPointer(indexColor, 4, GL_FLOAT, GL_FALSE, sizeof(s_lineVertex), &bufferTri[0].rgba);
	glDrawElements(GL_TRIANGLES, indicesTri.size(), GL_UNSIGNED_SHORT, &indicesTri[0]);

	// render world-space line primitives:
	glVertexAttribPointer(indexPos, 3, GL_FLOAT, GL_FALSE, sizeof(s_lineVertex), &buffer[0].pos);
	glVertexAttribPointer(indexColor, 4, GL_FLOAT, GL_FALSE, sizeof(s_lineVertex), &buffer[0].rgba);
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);

	// set up viewport space settings:
	float sx = 2.f / (vp->width()-1);
	float sy = -2.f / (vp->height()-1);
	float sz = -1.e-2f;
	glm::mat4x4 matVP_to_UniformScale(glm::scale(glm::mat4(), glm::vec3(sx, sy, sz)));
	int vpw = vp->width(), vph = vp->height();
	glm::mat4x4 matVP_to_Uniform(glm::translate(matVP_to_UniformScale,
			glm::vec3(-vpw/2, -vph/2, 0)));
	glUniformMatrix4fv(indexMatViewProj, 1, GL_FALSE, glm::value_ptr(matVP_to_Uniform));
	glEnableVertexAttribArray(indexPos);
	glEnableVertexAttribArray(indexColor);

	// render viewport-space triangle primitives:
	glVertexAttribPointer(indexPos, 3, GL_FLOAT, GL_FALSE, sizeof(s_lineVertex), &bufferTriVPSP[0].pos);
	glVertexAttribPointer(indexColor, 4, GL_FLOAT, GL_FALSE, sizeof(s_lineVertex), &bufferTriVPSP[0].rgba);
	glDrawElements(GL_TRIANGLES, indicesTriVPSP.size(), GL_UNSIGNED_SHORT, &indicesTriVPSP[0]);

	// render vieport-space line primitives:
	glVertexAttribPointer(indexPos, 3, GL_FLOAT, GL_FALSE, sizeof(s_lineVertex), &bufferVPSP[0].pos);
	glVertexAttribPointer(indexColor, 4, GL_FLOAT, GL_FALSE, sizeof(s_lineVertex), &bufferVPSP[0].rgba);
	glDrawElements(GL_LINES, indicesVPSP.size(), GL_UNSIGNED_SHORT, &indicesVPSP[0]);

	glDisable(GL_BLEND);
}

void Shape2D::purgeRenderQueue() {
	buffer.clear();
	bufferVPSP.clear();
	indices.clear();
	indicesVPSP.clear();
	bufferTri.clear();
	bufferTriVPSP.clear();
	indicesTri.clear();
	indicesTriVPSP.clear();
}

void Shape2D::drawLine(ViewportCoord point1, ViewportCoord point2, float z, glm::vec3 const &rgb) {
	drawLine(point1, point2, z, glm::vec4(rgb, 1));
}

void Shape2D::drawLine(ViewportCoord point1, ViewportCoord point2, float z, glm::vec4 const &rgba) {
	auto *pBuf = viewportSpaceEnabled_ ? &bufferVPSP : &buffer;
	auto *pInd = viewportSpaceEnabled_ ? &indicesVPSP : &indices;
	s_lineVertex s;
	// point1:
	s.pos = glm::vec3(point1, z);
	s.rgba = rgba;
	pBuf->push_back(s);
	pInd->push_back(pBuf->size()-1);
	// point2:
	s.pos = glm::vec3(point2, z);
	pBuf->push_back(s);
	pInd->push_back(pBuf->size()-1);
}

void Shape2D::drawLineList(ViewportCoord verts[], int nVerts, float z, glm::vec3 const &rgb) {
	drawLineList(verts, nVerts, z, glm::vec4(rgb, 1));
}

void Shape2D::drawLineList(ViewportCoord verts[], int nVerts, float z, glm::vec4 const &rgba) {
	auto *pBuf = viewportSpaceEnabled_ ? &bufferVPSP : &buffer;
	auto *pInd = viewportSpaceEnabled_ ? &indicesVPSP : &indices;
	s_lineVertex s;
	for (int i=0; i<nVerts; i++) {
		s.pos = glm::vec3(verts[i], z);
		s.rgba = rgba;
		pBuf->push_back(s);
		pInd->push_back(pBuf->size()-1);
	}
}

void Shape2D::drawLineStrip(ViewportCoord verts[], int nVerts, float z, glm::vec3 const &rgb) {
	drawLineStrip(verts, nVerts, z, glm::vec4(rgb, 1));
}

void Shape2D::drawLineStrip(ViewportCoord verts[], int nVerts, float z, glm::vec4 const &rgba) {
	auto *pBuf = viewportSpaceEnabled_ ? &bufferVPSP : &buffer;
	auto *pInd = viewportSpaceEnabled_ ? &indicesVPSP : &indices;
	s_lineVertex s;
	for (int i=0; i<nVerts; i++) {
		s.pos = glm::vec3(verts[i], z);
		s.rgba = rgba;
		pBuf->push_back(s);
		pInd->push_back(pBuf->size()-1);
		if (i > 0 && i < nVerts-1)
			pInd->push_back(pBuf->size()-1);
	}
}

void Shape2D::drawPolygon(ViewportCoord verts[], int nVerts, float z, glm::vec3 const &rgb) {
	drawPolygon(verts, nVerts, z, glm::vec4(rgb, 1));
}

void Shape2D::drawPolygon(ViewportCoord verts[], int nVerts, float z, glm::vec4 const &rgba) {
	auto *pBuf = viewportSpaceEnabled_ ? &bufferVPSP : &buffer;
	auto *pInd = viewportSpaceEnabled_ ? &indicesVPSP : &indices;
	s_lineVertex sVertex;
	sVertex.rgba = rgba;
	for (int i=0; i<nVerts; i++) {
		sVertex.pos = glm::vec3(verts[i], z);
		pBuf->push_back(sVertex);
		pInd->push_back(pBuf->size()-1);
		if (i > 0)
			pInd->push_back(pBuf->size()-1);
	}
	pInd->push_back(pBuf->size()-nVerts);
}

void Shape2D::drawPolygonFilled(ViewportCoord verts[], int nVerts, float z, glm::vec3 const &rgb) {
	drawPolygonFilled(verts, nVerts, z, glm::vec4(rgb, 1));
}

void Shape2D::drawPolygonFilled(ViewportCoord verts[], int nVerts, float z, glm::vec4 const &rgba) {
	//TODO must tesselate into triangles
}

void Shape2D::drawRectangle(ViewportCoord pos, float z, glm::vec2 const &size, glm::vec3 const &rgb) {
	drawRectangle(pos, z, size, glm::vec4(rgb, 1));
}

void Shape2D::drawRectangle(ViewportCoord pos, float z, glm::vec2 const &size, glm::vec4 const &rgba) {
	auto *pBuf = viewportSpaceEnabled_ ? &bufferVPSP : &buffer;
	auto *pInd = viewportSpaceEnabled_ ? &indicesVPSP : &indices;
	s_lineVertex sVertex;
	sVertex.rgba = rgba;
	// top left
	sVertex.pos = glm::vec3(pos, z);
	pBuf->push_back(sVertex);
	pInd->push_back(pBuf->size()-1);
	// top right
	sVertex.pos = glm::vec3(pos+glm::vec2(size.x, 0), z);
	pBuf->push_back(sVertex);
	pInd->push_back(pBuf->size()-1);
	pInd->push_back(pBuf->size()-1);
	// bottom right
	sVertex.pos = glm::vec3(pos+size, z);
	pBuf->push_back(sVertex);
	pInd->push_back(pBuf->size()-1);
	pInd->push_back(pBuf->size()-1);
	// bottom left
	sVertex.pos = glm::vec3(pos+glm::vec2(0, size.y), z);
	pBuf->push_back(sVertex);
	pInd->push_back(pBuf->size()-1);
	pInd->push_back(pBuf->size()-1);
	// top left again
	pInd->push_back(pBuf->size()-4);
}

void Shape2D::drawRectangleCentered(ViewportCoord pos, float z, glm::vec2 const &size, float rotation, glm::vec3 const &rgb) {
	drawRectangleCentered(pos, z, size, rotation, glm::vec4(rgb, 1));
}

void Shape2D::drawRectangleCentered(ViewportCoord pos, float z, glm::vec2 const &size, float rotation, glm::vec4 const &rgba) {
	auto *pBuf = viewportSpaceEnabled_ ? &bufferVPSP : &buffer;
	auto *pInd = viewportSpaceEnabled_ ? &indicesVPSP : &indices;
	float halfW = size.x * 0.5f;
	float halfH = size.y * 0.5f;
	s_lineVertex sVertex;
	sVertex.rgba = rgba;
	// top left
	sVertex.pos = glm::vec3(glm::rotate(glm::vec2(-halfW, halfH), rotation) + pos, z);
	pBuf->push_back(sVertex);
	pInd->push_back(pBuf->size()-1);
	// top right
	sVertex.pos = glm::vec3(glm::rotate(glm::vec2(halfW, halfH), rotation) + pos, z);
	pBuf->push_back(sVertex);
	pInd->push_back(pBuf->size()-1);
	pInd->push_back(pBuf->size()-1);
	// bottom right
	sVertex.pos = glm::vec3(glm::rotate(glm::vec2(halfW, -halfH), rotation) + pos, z);
	pBuf->push_back(sVertex);
	pInd->push_back(pBuf->size()-1);
	pInd->push_back(pBuf->size()-1);
	// bottom left
	sVertex.pos = glm::vec3(glm::rotate(glm::vec2(-halfW, -halfH), rotation) + pos, z);
	pBuf->push_back(sVertex);
	pInd->push_back(pBuf->size()-1);
	pInd->push_back(pBuf->size()-1);
	// top left again
	pInd->push_back(pBuf->size()-4);
}

void Shape2D::drawRectangleFilled(ViewportCoord pos, float z, glm::vec2 const &size, glm::vec3 const &rgb) {
	drawRectangleFilled(pos, z, size, glm::vec4(rgb, 1));
}

void Shape2D::drawRectangleFilled(ViewportCoord pos, float z, glm::vec2 const &size, glm::vec4 const &rgba) {
	auto *pBuf = viewportSpaceEnabled_ ? &bufferTriVPSP : &bufferTri;
	auto *pInd = viewportSpaceEnabled_ ? &indicesTriVPSP : &indicesTri;
	s_lineVertex sVertex;
	sVertex.rgba = rgba;
	// top left
	sVertex.pos = glm::vec3(pos, z);
	pBuf->push_back(sVertex);
	pInd->push_back(pBuf->size()-1);
	// top right
	sVertex.pos = glm::vec3(pos+glm::vec2(size.x, 0), z);
	pBuf->push_back(sVertex);
	pInd->push_back(pBuf->size()-1);
	// bottom left
	sVertex.pos = glm::vec3(pos+glm::vec2(0, size.y), z);
	pBuf->push_back(sVertex);
	pInd->push_back(pBuf->size()-1);
	// bottom left
	pInd->push_back(pBuf->size()-1);
	// top right
	pInd->push_back(pBuf->size()-2);
	// bottom right
	sVertex.pos = glm::vec3(pos+size, z);
	pBuf->push_back(sVertex);
	pInd->push_back(pBuf->size()-1);
}

void Shape2D::drawCircle(ViewportCoord pos, float radius, float z, int nSides, glm::vec3 const &rgb) {
	drawCircle(pos, radius, z, nSides, glm::vec4(rgb, 1));
}

void Shape2D::drawCircle(ViewportCoord pos, float radius, float z, int nSides, glm::vec4 const &rgba) {
	// make a polygon out of the circle
	float phiStep = 2 * PI * 1.f / nSides;
	glm::vec2 *v = new glm::vec2[nSides];
	float phi = 0;
	for (int i=0; i<nSides; i++) {
		v[i].x = radius * cosf(phi) + pos.x;
		v[i].y = radius * sinf(phi) + pos.y;
		phi += phiStep;
	}
	drawPolygon(v, nSides, z, rgba);
	delete [] v;
}
