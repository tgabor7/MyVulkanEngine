#include "myve_renderer.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <unordered_map>
#include "input_handler.hpp"

namespace myve
{
	
	void Renderer::loadModel()
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "models/viking_room.obj")) {
			throw std::runtime_error(warn + err);
		}

		std::unordered_map<VertexData, uint32_t> uniqueVertices{};

		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				VertexData vertex{};

				vertex.pos = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};

				vertex.color = { 1.0f, 1.0f, 1.0f };

				if (uniqueVertices.count(vertex) == 0) {
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}

				indices.push_back(uniqueVertices[vertex]);
			}
		}

	}
	void Renderer::init()
	{
		loadModel();

		window = std::make_unique<Window>();
		device = std::make_unique<Device>(window->getInstance(), window->getSurface());
		swapchain = std::make_unique<Swapchain>(*device, window->getWindow());
		texture = std::make_unique<Texture>(*device);

		InputHandler::init(window->getWindow());

		vbo = std::make_unique<VBO>(*device, vertices, indices);
		ubo = std::make_unique<UBO>(*device, *swapchain, *texture);

		pipeline = std::make_unique<Pipeline>(*device, *swapchain, window->getWindow(), *vbo, *ubo);
	}
	Renderer::Renderer()
	{
		init();
	}
	Renderer::~Renderer()
	{

	}
	void Renderer::draw()
	{
		while (!window->shouldClose()) {
			window->pollEvents();
			pipeline->drawFrame();
		}
	}
	void Renderer::cleanUp()
	{
		pipeline->cleanUp();
		vbo->cleanUp();
		swapchain->cleanUp();
		device->cleanUp();
		window->cleanUp();
	}
}