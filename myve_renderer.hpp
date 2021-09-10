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

		void draw();

	private:

		std::vector<VertexData> vertices;
		std::vector<uint32_t> indices;
		
		Window window{};

		Device device{ window};
		Swapchain swapchain{ device, window};

		Pipeline *pipeline;
		std::unique_ptr<VBO> vbo;
		std::unique_ptr<UBO> ubo;
		std::unique_ptr<Texture> texture;
	};
}