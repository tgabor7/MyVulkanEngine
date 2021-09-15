#include "myve_model.hpp"

namespace myve
{
	Model::Model(Device& device, const ModelData& data, std::shared_ptr<Texture> texture) : texture{texture}
	{
		vbo = std::make_unique<VBO>(device, data.vertices, data.indices);
	}
	void Model::draw(const VkCommandBuffer &commandBuffer)
	{
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(vbo->getIndexCount()), 1, 0, 0, 0);
	}
	void Model::bind(const VkCommandBuffer &commandBuffer)
	{
		vbo->bind(commandBuffer);
	}
	Model::~Model()
	{

	}
}