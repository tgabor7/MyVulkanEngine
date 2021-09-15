#include "myve_mesh_renderer.hpp"
#include <iostream>
namespace myve
{
	MeshRenderer::MeshRenderer(const std::string shaderPath, Device& device, Swapchain& swapchain, const VkRenderPass& renderPass) : SimpleRenderer(shaderPath, device, swapchain, renderPass)
	{
		ModelData data = ResourceLoader::loadOBJ("models/bunny.obj");
		std::shared_ptr<Texture> texture = std::make_shared<Texture>(device, "textures/bunny-atlas.jpg");

		texture->createTextureImage();
		texture->createTextureImageView();
		texture->createTextureSampler();
		Model m{ device, data, texture };

		m.vbo->createVertexBuffer();
		m.vbo->createIndexBuffer();

		m.transformation.scale = glm::vec3{ .01, .01, .01 };

		addMesh(m);

		m.transformation.position.x = .5;
		m.transformation.rotation.x = 1.56;
		for (int i = -20; i < 20; i++) {
			for (int j = -20; j < 20; j++) {
				m.transformation.position.x = 5.0 * i;
				m.transformation.position.y = 5.0 * j;
				addMesh(m);
			}

		}
		camera.translate({ 0,0,20 });
		ubo->changeTexture(*texture);
	}
	void MeshRenderer::draw(const VkCommandBuffer& commandBuffer, uint32_t currentImage)
	{
		shader->bind(commandBuffer);

		ubo->bind(commandBuffer, pipelineLayout, currentImage);

		for (Model& m : models)
		{
			PushConstantData d;
			d.model = m.transformation.getMatrix();
			shader->pushConstants(commandBuffer, sizeof(PushConstantData), d);

			m.bind(commandBuffer);
			m.draw(commandBuffer);
		}
		InputHandler::update();
	}
	void MeshRenderer::update(const VkCommandBuffer& commandBuffer, uint32_t currentImage)
	{
		UniformBufferObject ubo_data;

		ubo_data.proj = camera.getProjectionMatrix(70.0, 0.1, 100.0);
		ubo_data.view = camera.getViewMatrix();

		ubo->update(currentImage, ubo_data);

		
		if (InputHandler::isKeyDown(GLFW_KEY_UP)) {
			camera.movePitch(0.1);
		}
		if (InputHandler::isKeyDown(GLFW_KEY_DOWN)) {
			camera.movePitch(-0.1);
		}
		if (InputHandler::isKeyDown(GLFW_KEY_LEFT)) {
			camera.moveYaw(0.1);
		}
		if (InputHandler::isKeyDown(GLFW_KEY_RIGHT)) {
			camera.moveYaw(-0.1);
		}
		if (InputHandler::isKeyDown(GLFW_KEY_A)) {
			camera.translate({ .1,0,0 });
		}
		if (InputHandler::isKeyDown(GLFW_KEY_D)) {
			camera.translate({ -.1,0,0 });

		}
		if (InputHandler::isKeyDown(GLFW_KEY_W)) {
			camera.translate({ 0,-.1,0 });

		}
		if (InputHandler::isKeyDown(GLFW_KEY_S)) {
			camera.translate({ 0,.1,0 });

		}


	}
	void MeshRenderer::initialize()
	{

	}
	void MeshRenderer::addMesh(const Model& model)
	{
		models.push_back(model);
	}

}