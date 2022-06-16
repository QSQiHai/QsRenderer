#include "renderer.h"
#include "tgaimage.h"
#include "shader.h"

Renderer::Renderer(Camera& camera, Light& light, std::vector<Model*>& modelArray, int width, int height) :
	camera(camera), light(light), modelArray(modelArray), width(width), height(height), zBuffer(width, height, 1e10), frameBuffer(width, height)
{}

mat<4, 4> Renderer::GetModelMatrix(const Model& model) const {
	return mat<4, 4>::identity();
}

mat<4, 4> Renderer::GetCameraViewMatrix() const {
	mat<4, 4> viewMatrix;

	vec3 z = (camera.lookatPos - camera.cameraPos).normalize();
	vec3 x = cross(z, camera.upDir).normalize();
	vec3 y = cross(x, z).normalize();
	mat<4, 4> Minv = { {
		{x.x, x.y, x.z, 0},
		{y.x, y.y, y.z, 0},
		{-z.x, -z.y, -z.z, 0},
		{0, 0, 0, 1}
	} };
	mat<4, 4> Tr = { {
		{1, 0, 0, -camera.cameraPos.x},
		{0, 1, 0, -camera.cameraPos.y},
		{0, 0, 1, -camera.cameraPos.z},
		{0, 0, 0, 1}
	} };
	viewMatrix = Minv * Tr;
	return viewMatrix;
}

mat<4, 4> Renderer::GetCameraProjectMatrix() const {
	double fovY = camera.fovY;
	double aspect = camera.aspect;
	double zNear = camera.zNear;
	double zFar = camera.zFar;

	const double t = tan(fovY / 2) * zNear;
	const double r = t * aspect;
	mat<4, 4> projectMatrix = { {
		{zNear / r, 0, 0, 0},
		{0, zNear / t, 0, 0},
		{0, 0, -(zFar + zNear) / (zFar - zNear), -(2 * zNear * zFar) / (zFar - zNear)},
		{0, 0, -1, 0}
	} };
	return projectMatrix;
}

vec3  Renderer::GetCameraPos() const {
	return camera.cameraPos;
}

vec3 Renderer::GetWorldSpaceLightDir() const {
	return light.lightDir.normalize();
}

mat<4, 4> Renderer::GetViewportMatrix() const {
	return mat<4, 4> { {
		{ width / 2.0, 0, 0, width / 2.0},
		{ 0, height / 2.0, 0, height / 2.0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
		} };
}

vec3 Renderer::barycentric(const vec2 tri[3], const vec2 P) const {
	mat<3, 3> tmpMatrix = { {embed<3>(tri[0]), embed<3>(tri[1]), embed<3>(tri[2])} };
	if (tmpMatrix.det() < 1e-3) return vec3(-1, 1, 1);
	return tmpMatrix.invert_transpose() * embed<3>(P);
}

void Renderer::GetTexture(const Model& model, const std::string suffix, TGAImage& output_img) {
	std::string filename = model.GetFilename();
	size_t dot = filename.find_last_of(".");
	if (dot == std::string::npos) return;
	std::string texfile = filename.substr(0, dot) + suffix;
	bool ok = output_img.read_tga_file(texfile.c_str());
	std::cerr << "texture file " << texfile << " loading " << (ok ? "ok" : "failed") << std::endl;
	if (!ok) exit(0);
}

void Renderer::RenderMainFun() {

	for (Model* model : modelArray) {
		BlinnPhongShader shader(*this, *model);

		int n = model->GetNumberOfFaces();
		for (int i = 0; i < n; i++) {
			vec4 clipPos[3];
			for (int j = 0; j < 3; j++) {
				shader.PreWork(i, j);
				clipPos[j] = shader.vertex(j);
			}

			// Homogeneous division
			for (int j = 0; j < 3; j++) {
				double tmpw = clipPos[j][3];
				clipPos[j] = clipPos[j] / tmpw;
				clipPos[j][3] = tmpw;
			}

			// Viewport Transform
			mat<4, 4> viewportMatrix = GetViewportMatrix();
			vec2 screenPos[3];

			for (int j = 0; j < 3; j++) {
				screenPos[j] = proj<2>(viewportMatrix * embed<4>(proj<2>(clipPos[j])));
			}

			// Construct AABB
			vec2 bboxmin(1e10, 1e10);
			vec2 bboxmax(-1e10, -1e10);

			for (int i = 0; i < 3; i++) {
				bboxmin.x = screenPos[i].x < bboxmin.x ? screenPos[i].x : bboxmin.x;
				bboxmin.y = screenPos[i].y < bboxmin.y ? screenPos[i].y : bboxmin.y;
				bboxmax.x = screenPos[i].x > bboxmax.x ? screenPos[i].x : bboxmax.x;
				bboxmax.y = screenPos[i].y > bboxmax.y ? screenPos[i].y : bboxmax.y;
			}
			bboxmin.x = 0 > bboxmin.x ? 0 : bboxmin.x;
			bboxmin.y = 0 > bboxmin.y ? 0 : bboxmin.y;
			bboxmax.x = (width - 1.) < bboxmax.x ? (width - 1.) : bboxmax.x;
			bboxmax.y = (height - 1.) < bboxmax.y ? (height - 1.) : bboxmax.y;

			// Rasterization
#pragma omp parallel for
			for (int x = (int)bboxmin.x; x <= (int)bboxmax.x; x++) {
				for (int y = (int)bboxmin.y; y <= (int)bboxmax.y; y++) {
					vec3 bc_screen = barycentric(screenPos, vec2(x, y));
					vec3 bc_clip = vec3(bc_screen.x / clipPos[0][3], bc_screen.y / clipPos[1][3], bc_screen.z / clipPos[2][3]);
					double frag_depth = 1 / (bc_clip.x + bc_clip.y + bc_clip.z);
					bc_clip = bc_clip / (bc_clip.x + bc_clip.y + bc_clip.z);

					if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z<0 || frag_depth > zBuffer.GetValue(x, y)) continue;
					vec3 color;
					if (shader.fragment(bc_clip, color)) continue;
					zBuffer.SetValue(x, y, frag_depth);
					frameBuffer.SetValue(x, y, color);
				}
			}
		}
	}

	TGAImage outputImage(width, height, TGAImage::RGB);
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			vec3 val = frameBuffer.GetValue(x, y);
			TGAColor color(std::uint8_t(val.x * 255), std::uint8_t(val.y * 255), std::uint8_t(val.z * 255));
			outputImage.set(x, y, color);
		}
	}
	outputImage.write_tga_file("output.tga");

	return;
}