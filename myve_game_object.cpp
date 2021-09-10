#include "myve_game_object.hpp"

namespace myve
{
	uint32_t GameObject::id = 0;
	
	

	GameObject::GameObject(Device& device, Swapchain& swapchain) : device{ device }, swapchain{swapchain}
	{
		ModelData data = ResourceLoader::loadOBJ("models/bunny.obj");

		texture = new Texture(device, "textures/bunny-atlas.jpg");

		vbo = new VBO(device, data.vertices, data.indices);
		ubo = new UBO(device, swapchain, *texture);
	}
	GameObject::GameObject(Device& device, Swapchain& swapchain, const ModelData& data, Texture* _texture) : device{device}, swapchain{swapchain}, texture{_texture}
	{
		vbo = new VBO(device, data.vertices, data.indices);
		ubo = new UBO(device, swapchain, *texture);
	}
	GameObject::~GameObject()
	{

	}
}