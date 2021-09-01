#pragma once
#include "myve_window.hpp"
#include "myve_device.hpp"
#include "myve_pipeline.hpp"
#include "myve_swapchain.hpp"
#include "myve_shader.hpp"
#include "myve_vbo.hpp"
#include "myve_ubo.hpp"

namespace myve
{
	class Renderer {
	public:

		Renderer();
		~Renderer();

		void init();
		void cleanUp();

		void draw();

	private:
		std::unique_ptr<Device> device;
		std::unique_ptr<Window> window;
		std::unique_ptr<Swapchain> swapchain;
		std::unique_ptr<Pipeline> pipeline;
		std::unique_ptr<VBO> vbo;
		std::unique_ptr<UBO> ubo;
	};
}