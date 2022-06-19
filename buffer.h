#pragma once

#include <vector>
#include <string>
#include "geometry.h"
#include "model.h"

template<class T> 
struct Buffer {
	int width, height;
	std::vector<T> buffer;
	Buffer() = default;
	Buffer(int n, int m) : width(n), height(m), buffer(n* m) {}
	Buffer(int n, int m, T val) : width(n), height(m), buffer(n* m, val) {}
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	int GetSize() const { return buffer.size(); }
	int GetIdx(int x, int y) const { return x + y * width; }
	T GetValue(int x, int y) const { return buffer[GetIdx(x, y)]; }
	T GetValue(int idx) const { return buffer[idx]; }
	void SetValue(int x, int y, T val) { buffer[GetIdx(x, y)] = val; }
	void SetValue(int idx, T val) { buffer[idx] = val; }
};

