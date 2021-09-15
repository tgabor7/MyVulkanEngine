#pragma once 
#include "myve_resource_loader.hpp"
#include "myve_vbo.hpp"
#include "myve_game_object.hpp"

namespace myve
{
	class Model 
	{
	public:
		Model(Device& device, const ModelData &data, std::shared_ptr<Texture> texture);
		void draw(const VkCommandBuffer &commandBuffer);
		void bind(const VkCommandBuffer &commandBuffer);
		Transformation transformation;
		std::shared_ptr<VBO> vbo;
		~Model();
	private:
		ModelData data;
		std::shared_ptr<Texture> texture;
	};
}