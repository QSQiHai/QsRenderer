#pragma once

#include "geometry.h"
#include "renderer.h"
#include "tgaimage.h"

class Shader {
protected:
	Model& model;
	mat<4, 4> modelMatrix;
	mat<4, 4> mvpMatrix;
	vec3 worldSpaceLightDir;
	vec3 cameraPos;

public :
	vec3 saturate(vec3 vec) {
		return vec3(saturate(vec.x), saturate(vec.y), saturate(vec.z));
	}
	double saturate(double x) {
		if (x < 0) return 0.0;
		else if (x >= 1.) return 0.9998;
		return x;
	}
	vec3 tex2D(TGAImage& tex, vec2 pos) {
		int x = (int)(saturate(pos.x) * tex.width());
		int y = (int)(saturate(pos.y) * tex.height());
		TGAColor color = tex.get(x, y);
		return vec3((double)color[2] / 255, (double)color[1] / 255, (double)color[0] / 255);
	}


	Shader(Renderer& renderer, Model& model) : model(model) {
		modelMatrix = renderer.GetModelMatrix(model);
		mvpMatrix = renderer.GetCameraProjectMatrix() * renderer.GetCameraViewMatrix() * modelMatrix;
		worldSpaceLightDir = renderer.GetWorldSpaceLightDir();
		cameraPos = renderer.GetCameraPos();
	}
	
	virtual bool fragment(vec3 bar, vec3& out_color) = 0;
};

class BlinnPhongShader : public Shader {
	TGAImage mainTexture;

	vec3 input_vertex;
	vec3 input_normal;
	vec2 input_texcoord;

	vec4 varying_pos[3];
	mat<2, 3> varying_uv;
	mat<3, 3> varying_worldNormal;
	mat<3, 3> varying_worldPos;

public :
	BlinnPhongShader(Renderer& renderer, Model& model) : Shader(renderer, model) {
		renderer.GetTexture(model, "_main.tga", mainTexture);
	}

	void PreWork(const int iface, const int jvert) {
		input_vertex = model.GetVert(iface, jvert);
		input_normal = model.GetNormal(iface, jvert).normalize();
		input_texcoord = model.GetTexcoord(iface, jvert);
	}

	vec4 vertex(const int jvert) {
		varying_pos[jvert] = mvpMatrix * embed<4>(input_vertex);
		varying_uv.set_col(jvert, input_texcoord);
		varying_worldNormal.set_col(jvert, proj<3>((modelMatrix).invert_transpose() * embed<4>(input_normal, 0.)).normalize());
		varying_worldPos.set_col(jvert, proj<3>(modelMatrix * embed<4>(input_vertex)));

		return varying_pos[jvert];
	}

	bool fragment(vec3 bar, vec3& out_color) {
		vec2 uv = varying_uv * bar;
		vec3 worldNormal = (varying_worldNormal * bar).normalize();
		vec3 worldPos = varying_worldPos * bar;

		vec3 viewDir = (cameraPos - worldPos).normalize();

		const double ambLight = 10.0 / 255;
		double diffuse = saturate(worldSpaceLightDir * worldNormal);
		double specular = pow(std::max((viewDir + worldSpaceLightDir).normalize() * worldNormal, 0.0), 15);
		vec3 color = tex2D(mainTexture, uv);
		out_color = saturate(color * (diffuse + specular + ambLight));

		return false;
	}
};


