#pragma once

#include "geometry.h"

// Parallel light
class Light {
public :
	vec3 lightPos, lightDir, lightColor;
	Light();
	Light(vec3 lightDir);
};