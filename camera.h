#pragma once

#include "geometry.h"

class Camera {
public:
	vec3 cameraPos, lookatPos;
	vec3 upDir = { 0, 1, 0 };
	double fovY, aspect, zNear, zFar;

	Camera() = default;
	Camera(const vec3& cameraPos, const vec3& lookatPos);
	Camera(const vec3& cameraPos, const vec3& lookatPos, double fovY, double aspect, double zNear, double zFar);
};