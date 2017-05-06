/*
 * SignalViewer.h
 *
 *  Created on: Jan 24, 2016
 *      Author: bog
 */

#ifndef OSD_SIGNALVIEWER_H_
#define OSD_SIGNALVIEWER_H_

#include "../renderOpenGL/ViewportCoord.h"

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <memory>
#include <vector>
#include <type_traits>
#include <functional>

class SignalDataSource;

class SignalViewer {
public:

	struct DataInfo {
		std::unique_ptr<SignalDataSource> source_;
		std::string name_;
		glm::vec3 color_;
		float lastMinValue_ = 0.f;
		float lastMaxValue_ = 0.f;
		float minUpperY_ = -1e20f;
		float maxLowerY_ = 1e20f;

		DataInfo() = default;
		DataInfo(DataInfo &&x) = default;
		DataInfo(std::unique_ptr<SignalDataSource> && source, std::string const& name, glm::vec3 const& color, float minUpperY, float maxLowerY)
			: source_(std::move(source)), name_(name), color_(color),
			  minUpperY_(minUpperY), maxLowerY_(maxLowerY) {
		}
	};

	SignalViewer(ViewportCoord pos, float z, ViewportCoord size);
	virtual ~SignalViewer();

	void addSignal(std::string const& name, float* pValue, glm::vec3 const& rgb, float sampleInterval, int maxSamples = 50, float minUpperY = -1e20f, float maxLowerY = 1e20f);
	void addSignal(std::string const& name, std::function<float()> getValue, glm::vec3 const& rgb, float sampleInterval, int maxSamples = 50, float minUpperY = -1e20f, float maxLowerY = 1e20f);

	template <class Callable,
		typename std::enable_if<std::is_same<typename std::result_of<Callable()>::type, float>::value>::type>
	void addSignal(std::string const& name, Callable c, glm::vec3 const& rgb, float sampleInterval, int maxSamples = 50, float minUpperY = -1e20f, float maxLowerY = 1e20f) {
		addSignal(name, [c]() { return c(); }, rgb, sampleInterval, maxSamples, minUpperY, maxLowerY);
	}

	void update(float dt);
	void draw();

private:
	std::vector<DataInfo> sourceInfo_;
	ViewportCoord pos_;
	float z_;
	ViewportCoord size_;
};

class SignalDataSource {
public:
	SignalDataSource(std::function<float()> getValue, int maxSamples, float sampleInterval);
	virtual ~SignalDataSource();
	void update(float dt);
	inline float getSample(unsigned i) { return samples_[(i+zero_)%n_]; }
	inline unsigned getNumSamples() { return n_; }
	inline unsigned getCapacity() { return capacity_; }
private:
	std::function<float()> getValue_ = nullptr;
	float* samples_ = nullptr;	// circular buffer
	unsigned n_ = 0;
	unsigned zero_ = 0;
	unsigned capacity_ = 0;
	float sampleInterval_ = 0;
	float timeSinceLastSample_ = 0;
};

#endif /* OSD_SIGNALVIEWER_H_ */
