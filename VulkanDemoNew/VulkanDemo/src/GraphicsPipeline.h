#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <unordered_map>

#include "VkHelpers.h"
#include "Utilities.h"
#include "Vertex.h"
class GraphicsPipeline
{
private:
	VkDevice device;
	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;
	std::unordered_map< VkShaderStageFlagBits, std::vector<char>> rawShaders;
	



public:
	inline VkPipeline GetPipeline() { return graphicsPipeline; }
	inline VkPipelineLayout GetPipelineLayout() { return pipelineLayout; }

	void AddShaderRaw(VkShaderStageFlagBits stage, std::vector<char> rawShader);
	void Init(VkDevice dev, VkRenderPass renderPass, VkDescriptorSetLayout descriptorSetLayout, Vertex* format);
	void Destroy();
};
