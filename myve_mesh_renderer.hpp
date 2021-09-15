#include "myve_simple_renderer.hpp"
#include "myve_model.hpp"

namespace myve
{
	class MeshRenderer : public SimpleRenderer
	{
	public:
		MeshRenderer(const std::string shaderPath, Device& device, Swapchain& swapchain, const VkRenderPass& renderPass);
		void initialize();
		void draw(const VkCommandBuffer &commandBuffer, uint32_t currentImage);
		void addMesh(const Model& model);
		void update(const VkCommandBuffer& commandBuffer, uint32_t currentImage); //move this to scene later
		~MeshRenderer() {}
	private:
		Camera camera;
		std::vector<Model> models;
	};
}