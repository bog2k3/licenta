/*
 * ViewportCoord.h
 *
 *  Created on: May 6, 2017
 *      Author: alexandra
 */

#ifndef RENDEROPENGL_VIEWPORTCOORD_H_
#define RENDEROPENGL_VIEWPORTCOORD_H_

#include <functional>

class Viewport;

class ViewportCoord {
public:
	enum anchors {
		left		= 1,
		right		= 2,
		top			= 4,
		bottom		= 8
	};
	enum type {
		absolute,		// absolute viewport coordinates (in pixels)
		percent,		// coordinates are percentages of viewport size
		callback		// coordinates will be provided by user callback on a per-viewport basis
	};

	using userCallback = std::function<float(Viewport* vp)>;

	// builds an absolute coordinate (in pixels) or relative (in percents of viewport size)
	ViewportCoord(float x, float y, anchors a = (anchors)(left | top), type t = absolute)
		: anchor_(a), type_(t), x_(x), y_(y) {
	}

	// builds a deferred coordinate representation that will be computed at the actual time of rendering, on a per-viewport basis
	// the callbacks must return absolute values (anchors still apply)
	ViewportCoord(userCallback x, userCallback y, anchors a = (anchors)(left | top))
		: anchor_(a), type_(callback), xc_(x), yc_(y) {
	}

	// creates a new coordinate representation translated by the given amount
	// the values are interpreted according to the coordinate type
	ViewportCoord adjust(float dx, float dy) const;

	float x(Viewport* vp) const;
	float y(Viewport* vp) const;

private:
	anchors anchor_;
	type type_;
	float x_ = 0;
	float y_ = 0;
	userCallback xc_;
	userCallback yc_;

	ViewportCoord(anchors a, type t, float x, float y, userCallback xc, userCallback yc)
		: anchor_(a), type_(t), x_(x), y_(y), xc_(xc), yc_(yc) {
	}
};

#endif /* RENDEROPENGL_VIEWPORTCOORD_H_ */
