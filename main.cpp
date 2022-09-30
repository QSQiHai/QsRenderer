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

	std::string str = "try to rebuild my renderer";

	Camera camera(vec3(0.8, 0.8, 2.4), vec3(0, 0, 0));
	Light light(vec3(1, 1, 1));

	Renderer QsRenderer(camera, light, modelArray, 800, 800);
	QsRenderer.RenderMainFun();

	return 0;
}
