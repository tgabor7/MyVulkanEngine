#include "myve_scene.hpp"

namespace myve
{
	Scene::Scene()
	{
	}
	void Scene::run()
	{
		loadResources();
		renderer.init();

		while (!renderer.shouldClose()) {
			renderer.draw();
			update();
		}

	}
	void Scene::createGameObject()
	{
		renderer.createGameObject();
	}
	Scene::~Scene()
	{
	}
}