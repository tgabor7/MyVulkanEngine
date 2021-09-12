#pragma once
#include "myve_device.hpp"
#include "myve_swapchain.hpp"
#include "myve_ubo.hpp"

//std
#include <string>

namespace myve
{
	struct ShaderInfo {
		std::vector<VkPipelineShaderStageCreateInfo> pStages;
		VkPipelineVertexInputStateCreateInfo* inputInfo;
		VkPipelineInputAssemblyStateCreateInfo* assemblyInfo;
		VkPipelineViewportStateCreateInfo* viewportInfo;
		VkPipelineRasterizationStateCreateInfo* rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo* multisampleInfo;
		VkPipelineColorBlendStateCreateInfo* colorBlendInfo;
	};

	class Shader {
	public:
		Shader(Device &device,Swapchain &swapchain, const std::string &path, VkPipelineLayout pipelineLayout, const VkRenderPass &renderPass);
		ShaderInfo getShaderInfo() { return shaderInfo; }
		VkPipeline getGraphicsPipeline() const { return graphicsPipeline; }
		~Shader();
	private:
		static std::vector<char> readFile(const std::string& filename);
		VkShaderModule createShaderModule(const std::vector<char>& code);

		VkPipeline graphicsPipeline;

		Device& device;
		Swapchain& swapchain;
		ShaderInfo shaderInfo;
	};
}