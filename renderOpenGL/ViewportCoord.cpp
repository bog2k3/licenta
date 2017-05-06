/*
 * ViewportCoord.cpp
 *
 *  Created on: May 6, 2017
 *      Author: alexandra
 */

#include "ViewportCoord.h"
#include "Viewport.h"

float ViewportCoord::x(Viewport* vp) const {
	float x = x_;
	if (type_ == callback) {
		if (xc_)
			x += xc_(vp);
	} else if (type_ == percent)
		x *= vp->width() * 0.01f;
	if (anchor_ & right)
		x = vp->width() - x;
	for (auto& a : deferredAdjustments_) {
		if (a.type == Adjustment::scale)
			x *= a.weight;
		else
			x += a.weight * a.value.x(vp);
	}
	return x;
}

float ViewportCoord::y(Viewport* vp) const {
	float y = y_;
	if (type_ == callback) {
		if (yc_)
			y += yc_(vp);
	} else if (type_ == percent)
		y *= vp->height() * 0.01f;
	if (anchor_ & bottom)
		y = vp->height() - y;
	for (auto& a : deferredAdjustments_) {
		if (a.type == Adjustment::scale)
			y *= a.weight;
		else
			y += a.weight * a.value.y(vp);
	}
	return y;
}

ViewportCoord ViewportCoord::adjust(float dx, float dy) const {
	return ViewportCoord { anchor_, type_, x_ + dx, y_ + dy, xc_, yc_ };
}

ViewportCoord ViewportCoord::adjust(glm::vec2 const& d) const {
	return adjust(d.x, d.y);
}

ViewportCoord ViewportCoord::scale(float s) const {
	auto ret = *this;
	ret.deferredAdjustments_.push_back({Adjustment::scale, {0, 0}, s});
	return ret;
}

ViewportCoord ViewportCoord::operator+(ViewportCoord const& x) const {
	auto ret = *this;
	ret.deferredAdjustments_.push_back({Adjustment::add, x, +1});
	return ret;
}

ViewportCoord ViewportCoord::operator-(ViewportCoord const& x) const {
	auto ret = *this;
	ret.deferredAdjustments_.push_back({Adjustment::add, x, -1});
	return ret;
}

ViewportCoord ViewportCoord::x() const {
	auto ret = *this;
	ret.y_ = 0;
	ret.yc_ = nullptr;
	for (auto &a : ret.deferredAdjustments_)
		a.value = a.value.x();
	return ret;
}

ViewportCoord ViewportCoord::y() const {
	auto ret = *this;
	ret.x_ = 0;
	ret.xc_ = nullptr;
	for (auto &a : ret.deferredAdjustments_)
		a.value = a.value.y();
	return ret;
}
