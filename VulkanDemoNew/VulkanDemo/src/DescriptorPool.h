#pragma once
#include "VkHelpers.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <stdexcept>

class DescriptorPool
{
private:

	VkDevice device;
	VkDescriptorPool descriptorPool;

	std::vector<VkDescriptorPoolSize> poolSizes;

public:

	DescriptorPool(VkDevice device);
	~DescriptorPool();

	VkDescriptorPool GetDescriptorPool() { return descriptorPool; }

	void AddDescriptorPoolSize(VkDescriptorType type, uint32_t count);
	void Init(uint32_t maximumSets);







};

