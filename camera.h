#pragma once

#include "geometry.h"

class Camera {
public:
	vec3 cameraPos, lookatPos, upDir;
	double fovY, aspect, zNear, zFar;

	Camera() = default;
	Camera(const vec3& cameraPos, const vec3& lookatPos, const vec3& upDir);
	Camera(const vec3& cameraPos, const vec3& lookatPos, const vec3& upDir, double fovY, double aspect, double zNear, double zFar);
};