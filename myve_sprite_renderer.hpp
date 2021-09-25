#include "myve_simple_renderer.hpp"

namespace myve
{
	typedef std::pair<Texture, glm::vec2> SpriteSheet;



	struct Sprite
	{
		Sprite(const SpriteSheet& spritesheet, const int& row, const int& column) : row{ row }, column{ column }, spriteSheet{ spritesheet }{}

		int row;
		int column;
		SpriteSheet spriteSheet;
	};
	class SpriteRenderer : public SimpleRenderer
	{
	public:
		SpriteRenderer(const std::string shaderPath, Device& device, Swapchain& swapchain, const VkRenderPass& renderPass);
		void initialize() {}
		void draw(const VkCommandBuffer& commandBuffer, uint32_t currentImage);
		void update(const VkCommandBuffer& commandBuffer, uint32_t currentImage);
		~SpriteRenderer() {}
	private:
		std::vector<Sprite> sprites;
	};
}