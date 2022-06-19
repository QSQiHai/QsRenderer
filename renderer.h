#pragma once

#include <vector>
#include "geometry.h"
#include "tgaimage.h"
#include "buffer.h"
#include "camera.h"
#include "light.h"
#include "model.h"

class Renderer {
	Camera &camera;
	Light &light;
	std::vector<Model*> &modelArray;

	int width, height;
	Buffer<double> zBuffer;
	Buffer<vec3> frameBuffer;

public :
	Renderer(Camera&, Light&, std::vector<Model*>&, int, int);

	void RenderMainFun();

	mat<4, 4> GetModelMatrix(const Model&) const;

	mat<4, 4> GetCameraViewMatrix() const;
	mat<4, 4> GetCameraProjectMatrix() const;
	vec3 GetCameraPos() const;
	
	vec3 GetWorldSpaceLightDir() const;
	mat<4, 4> GetViewportMatrix() const;

	void GetTexture(const Model&, const std::string suffix, TGAImage& output_img);

	vec3 barycentric(const vec2*, const vec2) const;
};