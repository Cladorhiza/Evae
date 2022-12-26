#pragma once
#include "VkHelpers.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <stdexcept>

class DescriptorSetLayout
{
private:

	VkDevice device;
	VkDescriptorSetLayout descriptorSetLayout;
	std::vector<VkDescriptorSetLayoutBinding> bindings;

public:

	DescriptorSetLayout(VkDevice device);
	~DescriptorSetLayout();

	VkDescriptorSetLayout GetDescriptorSetLayout() { return descriptorSetLayout; }

	void AddBinding(uint32_t bindingIndex, VkDescriptorType descriptorType, uint32_t count, VkShaderStageFlagBits stageFlags);
	void Init();
};

