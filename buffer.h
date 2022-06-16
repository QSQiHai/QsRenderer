#pragma once

#include "geometry.h"
#include <vector>

template<class T> 
class Buffer {
public :
	int width, height;
	std::vector<T> buffer;
	Buffer() = default;
	Buffer(int n, int m);
	Buffer(int n, int m, T val);

	int GetWidth() const;
	int GetHeight() const;

	int GetSize() const;
	int GetIdx(int, int) const;

	T GetValue(int, int) const;
	T GetValue(int) const;

	void SetValue(int, int, T);
	void SetValue(int, T);
};

template<class T>
Buffer<T>::Buffer(int n, int m) :
	width(n), height(m), buffer(n* m)
{}

template<class T>
Buffer<T>::Buffer(int n, int m, T val) :
	width(n), height(m), buffer(n* m, val)
{}

template<class T>
int Buffer<T>::GetWidth() const {
	return width;
}

template<class T>
int Buffer<T>::GetHeight() const {
	return height;
}

template<class T>
int Buffer<T>::GetSize() const {
	return buffer.size();
}

template<class T>
int Buffer<T>::GetIdx(int x, int y) const {
	return x + y * width;
}

template<class T>
T Buffer<T>::GetValue(int x, int y) const {
	int idx = GetIdx(x, y);
	return buffer[idx];
}

template<class T>
T Buffer<T>::GetValue(int idx) const {
	return buffer[idx];
}

template<class T>
void Buffer<T>::SetValue(int x, int y, T val) {
	int idx = GetIdx(x, y);
	buffer[idx] = val;
}

template<class T>
void Buffer<T>::SetValue(int idx, T val) {
	buffer[idx] = val;
}

