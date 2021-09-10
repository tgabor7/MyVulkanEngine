#pragma once
#include "myve_vbo.hpp"
#include "myve_ubo.hpp"
#include "myve_texture.hpp"
#include "myve_resource_loader.hpp"
#include "myve_swapchain.hpp"

namespace myve
{
	struct Transformation
	{
		glm::vec3 position{};
		glm::vec3 rotation{};
		glm::vec3 scale{1.0};

		glm::mat4 getMatrix()
		{
			glm::mat4 result{ 1.0f };

			result = glm::translate(result, position);
			result = glm::rotate(result, rotation.x, { 1.0, 0.0, 0.0 });
			result = glm::rotate(result, rotation.y, { 0.0, 1.0, 0.0 });
			result = glm::rotate(result, rotation.z, { 0.0, 0.0, 1.0 });
			result = glm::scale(result, scale);

			return result;
		}
	};
	class GameObject 
	{
	public:
		GameObject(Device& device, Swapchain& swapchain);
		GameObject(Device &device, Swapchain& swapchain, const ModelData &data, Texture* texture);
		static uint32_t id;

		Transformation transformation;

		VBO *vbo;
		UBO *ubo;

		~GameObject();
	private:
		
		Device &device;
		Swapchain &swapchain;


		Texture *texture;
		ModelData data;
	};
}