#include "myve_vbo.hpp"
#include <stdexcept>
#include <cstring>

namespace myve
{
    void VBO::cleanUp()
    {
        vkDestroyBuffer(device.getDevice(), indexBuffer, nullptr);
        vkFreeMemory(device.getDevice(), indexBufferMemory, nullptr);

        vkDestroyBuffer(device.getDevice(), vertexBuffer, nullptr);
        vkFreeMemory(device.getDevice(), vertexBufferMemory, nullptr);
        
    }
    VBO::~VBO()
    {
        cleanUp();
    }
    VBO::VBO(Device& device, const std::vector<VertexData>& data, const std::vector<uint32_t>& indices) : device{ device }, vertices{ data }, indices{indices}
    {
        //createVertexBuffer();
    }
    void VBO::createIndexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
	    std::memcpy(data, indices.data(), (size_t)bufferSize);
        vkUnmapMemory(device.getDevice(), stagingBufferMemory);

        device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

        device.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

        vkDestroyBuffer(device.getDevice(), stagingBuffer, nullptr);
        vkFreeMemory(device.getDevice(), stagingBufferMemory, nullptr);
    }
    void VBO::bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer vertexBuffers[] = { vertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }
	void VBO::createVertexBuffer()
	{
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(device.getDevice(), stagingBufferMemory);

        device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

        device.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

        vkDestroyBuffer(device.getDevice(), stagingBuffer, nullptr);
        vkFreeMemory(device.getDevice(), stagingBufferMemory, nullptr);
	}
}
