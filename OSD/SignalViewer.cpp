/*
 * SignalViewer.cpp
 *
 *  Created on: Jan 24, 2016
 *      Author: bog
 */

#include "SignalViewer.h"
#include "../renderOpenGL/RenderContext.h"
#include "../renderOpenGL/Shape2D.h"
#include "../renderOpenGL/GLText.h"
#include "../renderOpenGL/Viewport.h"
#include "../math/math3D.h"

#include <sstream>
#include <iomanip>

SignalDataSource::SignalDataSource(std::function<float()> getValue, int maxSamples, float sampleInterval)
	: getValue_(getValue), capacity_(maxSamples), sampleInterval_(sampleInterval) {
	samples_ = new float[maxSamples];
}

SignalDataSource::~SignalDataSource() {
	delete [] samples_;
}

void SignalDataSource::update(float dt) {
	timeSinceLastSample_ += dt;
	if (timeSinceLastSample_ < sampleInterval_)
		return;

	timeSinceLastSample_ -= sampleInterval_;

	if (n_ < capacity_)
		samples_[n_++] = getValue_();
	else {
		samples_[zero_] = getValue_();
		zero_ = (zero_+1) % n_;
	}
}

SignalViewer::SignalViewer(ViewportCoord pos, float z, ViewportCoord size)
	: pos_(pos), z_(z), size_(size) {
}

SignalViewer::~SignalViewer() {
}

void SignalViewer::addSignal(std::string const& name, float* pValue, glm::vec3 const& rgb, float sampleInterval, int maxSamples, float minUpperY, float maxLowerY) {
	addSignal(name, [pValue] { return *pValue; }, rgb, sampleInterval, maxSamples, minUpperY, maxLowerY);
}

void SignalViewer::addSignal(std::string const& name, std::function<float()> getValue, glm::vec3 const& rgb, float sampleInterval, int maxSamples, float minUpperY, float maxLowerY) {
	sourceInfo_.push_back(DataInfo(std::unique_ptr<SignalDataSource>(new SignalDataSource(getValue, maxSamples, sampleInterval)), name, rgb, minUpperY, maxLowerY));
}

void SignalViewer::update(float dt) {
	for (auto &s : sourceInfo_)
		s.source_->update(dt);
}

void SignalViewer::draw() {
	constexpr float yDivisionSize = 20; // pixels
	constexpr int maxYDivisions = 5;
	constexpr float textSize = 14;
	constexpr float spacePerChar = textSize/2; // pixels
	const glm::vec4 frameColor(1.f, 1.f, 1.f, 0.3f);
	const glm::vec4 divisionColor(1.f, 1.f, 1.f, 0.2f);
	const glm::vec4 divisionLabelColor(1.f, 1.f, 1.f, 0.6f);

	auto pos = pos_;

	for (auto &s : sourceInfo_) {
		Shape2D::get()->drawRectangle(pos, z_, size_, frameColor);
		std::stringstream stitle;
		stitle << s.name_;
		if (s.source_->getNumSamples())
			stitle << " : " << s.source_->getSample(s.source_->getNumSamples()-1);
		else
			stitle << " (no values)";
		GLText::get()->print(stitle.str(), pos, z_, textSize, s.color_);
		float sMin = 1.e20f, sMax = -1.e20f;
		// scan all samples and seek min/max values:
		for (unsigned i=0; i<s.source_->getNumSamples(); i++) {
			float si = s.source_->getSample(i);
			if (si < sMin)
				sMin = si;
			if (si > sMax)
				sMax = si;
		}
		// smooth out zoom level:
		if (sMin > s.lastMinValue_)
			sMin = sMin*0.1f + s.lastMinValue_*0.9f;
		if (sMax < s.lastMaxValue_)
			sMax = sMax*0.1f + s.lastMaxValue_*0.9f;
		s.lastMinValue_ = sMin;
		s.lastMaxValue_ = sMax;
		if (sMin > s.maxLowerY_)
			sMin = s.maxLowerY_;
		if (sMax < s.minUpperY_)
			sMax = s.minUpperY_;
		// draw samples:
		ViewportCoord xAxisZoom = size_.x() / s.source_->getCapacity();
		ViewportCoord yScale = size_.y() / (sMax - sMin);
		if (sMin == sMax)
			yScale = {0, 0};
		ViewportCoord prev = pos + size_.y() * 0.5f;
		for (unsigned i=0; i<s.source_->getNumSamples(); i++) {
			ViewportCoord crt = prev.x() + xAxisZoom +
					pos.y() + size_.y() - yScale * (s.source_->getSample(i)-sMin);
			Shape2D::get()->drawLine(prev, crt, z_, s.color_);
			prev = crt;
		}
		// draw value axis division lines & labels
		if (sMin * sMax < 0) {
			// zero line is visible
			auto zeroY = pos.y() + size_.y() + yScale*sMin;
			Shape2D::get()->drawLine(pos.x() + zeroY, pos.x() + size_.x() + zeroY, z_, frameColor);
		}
		int nYDivs = maxYDivisions; //min(maxYDivisions, (int)(size.y / yDivisionSize));
		int nDecimals = s.source_->getNumSamples() ? -log10(sMax - sMin) : 0;
		for (int i=1; i<nYDivs; i++) {
			auto lineY = pos.y() + size_.y().adjust(0, -i * yDivisionSize);
			Shape2D::get()->drawLine(pos.x() + lineY, pos.x() + size_.x() + lineY, z_, divisionColor);
			std::stringstream ss;
			ss << std::setprecision(nDecimals) << sMin + (sMax-sMin) * i / nYDivs;
			GLText::get()->print(ss.str(), pos.x().adjust((ss.str().size()+1) * spacePerChar, 5) + lineY, z_, textSize, divisionLabelColor);
		}

		pos = pos + size_.y().adjust(0, 15);
	}
}
