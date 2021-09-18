#pragma once
#include "myve_window.hpp"
#include "myve_device.hpp"
#include "myve_pipeline.hpp"
#include "myve_swapchain.hpp"
#include "myve_shader.hpp"
#include "myve_vbo.hpp"
#include "myve_ubo.hpp"
#include "myve_texture.hpp"

namespace myve
{
	class Renderer {
	public:

		Renderer();
		~Renderer();

		void init();
		void cleanUp();

		bool shouldClose();

		void draw();

		void createGameObject();

		void addGameObject(GameObject object);

	private:

		Window window{};

		Device device{ window};
		Swapchain swapchain{ device, window};

		Pipeline *pipeline;
	};
}