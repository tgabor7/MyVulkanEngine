#pragma once 
#include "myve_device.hpp"
#include "myve_vertex_data.hpp"

namespace myve
{
	class VBO
	{
	public:
		VBO(Device &device,const std::vector<VertexData> &data);
		void cleanUp();
		void bind(VkCommandBuffer commandBuffer);
		~VBO();
	private:
		Device& device;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		std::vector<VertexData> vertices;
		void createVertexBuffer();
	};
}