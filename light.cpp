#include "light.h"

Light::Light()
	: lightPos(vec3(0, 0, 0)), lightDir(vec3(1, 1, 1).normalize()), lightColor(vec3(1, 1, 1))
{}
