#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VkHelpers.h"
#include "Utilities.h"
#include "Vertex.h"

#include <unordered_map>
#include <iostream>
class GraphicsPipeline
{
private:

	VkDevice device;
	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;
	std::unordered_map< VkShaderStageFlagBits, std::vector<char>> rawShaders;
	



public:

	GraphicsPipeline();
	~GraphicsPipeline();

	VkPipeline GetPipeline() { return graphicsPipeline; }
	VkPipelineLayout GetPipelineLayout() { return pipelineLayout; }

	void AddShaderRaw(VkShaderStageFlagBits stage, std::vector<char> rawShader);
	void Init(VkDevice dev, VkRenderPass renderPass, VkDescriptorSetLayout descriptorSetLayout, Vertex* format);
};

