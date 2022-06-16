#include <vector>
#include "geometry.h"
#include "renderer.h"
#include "buffer.h"
#include "camera.h"
#include "light.h"
#include "model.h"

int main() {
	std::vector<Model*> modelArray;

	int n;
	std::cin >> n;
	for (int i = 0; i < n; i++) {
		std::string modelName;
		std::cin >> modelName;
		modelArray.emplace_back(new Model(modelName));
	}

	Camera camera(vec3(1 * 0.8, 1 * 0.8, 3 * 0.8), vec3(0, 0, 0), vec3(0, 1, 0));
	Light light;

	Renderer QsRenderer(camera, light, modelArray, 800, 800);
	QsRenderer.RenderMainFun();

	return 0;
}
