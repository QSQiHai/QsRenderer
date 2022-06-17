#include "light.h"

Light::Light()
	: lightPos(vec3(0, 0, 0)), lightDir(vec3(1, 1, 1).normalize()), lightColor(vec3(1, 1, 1))
{}

Light::Light(vec3 lightDir)
	: lightPos(vec3(0, 0, 0)), lightDir(lightDir), lightColor(vec3(1, 1, 1))
{}

