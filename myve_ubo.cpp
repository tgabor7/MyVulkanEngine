#include "myve_ubo.hpp"
#include <stdexcept>
#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include "input_handler.hpp"

namespace myve
{
    UBO::UBO(Device& device, Swapchain& swapchain, Texture& texture) : device{ device }, swapchain{ swapchain }, texture{texture}
	{

	}
	void UBO::createDescriptorSetLayout()
	{
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.pImmutableSamplers = nullptr;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(device.getDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
	}
    void UBO::createUniformBuffers()
    {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        uniformBuffers.resize(swapchain.getSwapChainImageCount());
        uniformBuffersMemory.resize(swapchain.getSwapChainImageCount());

        for (size_t i = 0; i < swapchain.getSwapChainImageCount(); i++) {
            device.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
        }
    }
    void UBO::createDescriptorPool()
    {
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = static_cast<uint32_t>(swapchain.getSwapChainImageCount());
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = static_cast<uint32_t>(swapchain.getSwapChainImageCount());

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(swapchain.getSwapChainImageCount());

        if (vkCreateDescriptorPool(device.getDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }
    void UBO::update(uint32_t currentImage, VkExtent2D swapChainExtent, glm::mat4 model)
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        
        ubo.model = model;
        
        ubo.view = c.getViewMatrix();
        
        ubo.proj = c.getProjectionMatrix(1.0f, 0.01f, 10.0f);

        if (InputHandler::isKeyDown(GLFW_KEY_A)) {
            c.translate({ -sin(c.yaw) * cos(c.pitch) * .1f,cos(c.yaw) * cos(c.pitch) * .1f, 0.f });

        }
        if (InputHandler::isKeyDown(GLFW_KEY_D)) {
            c.translate({ sin(c.yaw) * cos(c.pitch) * .1f,-cos(c.yaw) * cos(c.pitch) * .1f, 0.f });
        }
        if (InputHandler::isKeyDown(GLFW_KEY_S)) {
            c.translate({ -cos(c.yaw) * cos(c.pitch) * .1f, -sin(c.yaw) * cos(c.pitch)*.1f, 0.f });
        }
        if (InputHandler::isKeyDown(GLFW_KEY_W)) {
            c.translate({ cos(c.yaw) * cos(c.pitch)*.1f, sin(c.yaw) * cos(c.pitch)*.1f, 0.f });
        }
        if (InputHandler::isKeyDown(GLFW_KEY_E)) {
            c.translate({ 0.f,0.f, .1f });
        }
        if (InputHandler::isKeyDown(GLFW_KEY_Q)) {
            c.translate({ 0.f,0.f, -.1f });

        }
        Mouse m = InputHandler::getMouseD();

        /*c.moveYaw(-m.x * 0.01f);
        c.movePitch(m.y * 0.01f);*/

        void* data;
        vkMapMemory(device.getDevice(), uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(device.getDevice(), uniformBuffersMemory[currentImage]);

        InputHandler::update();

    }
    void UBO::bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, uint32_t currentImage)
    {
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentImage], 0, nullptr);
    }
    void UBO::createDescriptorSets()
    {
        std::vector<VkDescriptorSetLayout> layouts(swapchain.getSwapChainImageCount(), descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(swapchain.getSwapChainImageCount());
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets.resize(swapchain.getSwapChainImageCount());
        if (vkAllocateDescriptorSets(device.getDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t i = 0; i < swapchain.getSwapChainImageCount(); i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = texture.getImageView();
            imageInfo.sampler = texture.getSampler();

            std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = descriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = descriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo = &imageInfo;

            vkUpdateDescriptorSets(device.getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }
    }
    void UBO::cleanUp()
    {
        vkDestroyDescriptorSetLayout(device.getDevice(), descriptorSetLayout, nullptr);

        for (size_t i = 0; i < swapchain.getSwapChainImageCount(); i++) {
            vkDestroyBuffer(device.getDevice(), uniformBuffers[i], nullptr);
            vkFreeMemory(device.getDevice(), uniformBuffersMemory[i], nullptr);
        }
        vkDestroyDescriptorPool(device.getDevice(), descriptorPool, nullptr);
    }
	UBO::~UBO()
	{

	}
}