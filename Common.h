#ifndef COMMON_H
#define COMMON_H

#include <map>
#include <string>
#include <fstream>

template<typename T> struct rect {
	T x, y, w, h;

	rect() {}
	rect(T nx, T ny, T nwidth, T nheight) : x(nx), y(ny), w(nwidth), h(nheight) {}

	bool collides(const rect<T> &other) {
		if (x >= other.x + other.w) return false;
		if (x + w <= other.x) return false;
		if (y >= other.y + other.h) return false;
		if (y + h <= other.y) return false;
		
		return true;
	}
};

#endif