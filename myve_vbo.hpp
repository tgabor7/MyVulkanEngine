#pragma once 
#include "myve_device.hpp"
#include "myve_vertex_data.hpp"

namespace myve
{
	class VBO
	{
	public:
		VBO(Device &device,const std::vector<VertexData> &data, const std::vector<uint32_t> &indices);
		void cleanUp();
		void bind(VkCommandBuffer commandBuffer);
		void createVertexBuffer();
		void createIndexBuffer();
		size_t getIndexCount() const { return indices.size(); }
		~VBO();
	private:
		Device& device;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		std::vector<VertexData> vertices;

		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
		std::vector<uint32_t> indices;
	};
}