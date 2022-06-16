#pragma once

#include "camera.h"

Camera::Camera(const vec3& cameraPos, const vec3& lookatPos, const vec3& upDir) :
	cameraPos(cameraPos), lookatPos(lookatPos), upDir(upDir), fovY(45), aspect(1), zNear(0.1), zFar(50)
{}
Camera::Camera(const vec3& cameraPos, const vec3& lookatPos, const vec3& upDir, double fovY, double aspect, double zNear, double zFar) :
	cameraPos(cameraPos), lookatPos(lookatPos), upDir(upDir), fovY(fovY), aspect(aspect), zNear(zNear), zFar(zFar)
{}

