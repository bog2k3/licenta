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
	if (type_ == callback)
		x += xc_(vp);
	else if (type_ == percent)
		x *= vp->width() * 0.01f;
	if (anchor_ & right)
		x = vp->width() - x;
	return x;
}

float ViewportCoord::y(Viewport* vp) const {
	float y = y_;
	if (type_ == callback)
		y += yc_(vp);
	else if (type_ == percent)
		y *= vp->height() * 0.01f;
	if (anchor_ & bottom)
		y = vp->height() - y;
	return y;
}

ViewportCoord ViewportCoord::adjust(float dx, float dy) const {
	return ViewportCoord { anchor_, type_, x_ + dx, y_ + dy, xc_, yc_ };
}
