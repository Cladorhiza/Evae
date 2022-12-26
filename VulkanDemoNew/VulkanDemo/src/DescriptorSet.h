#pragma once
#include "VkHelpers.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <stdexcept>
class DescriptorSet
{
private:
	VkDevice device;
	VkDescriptorSet descriptorSet;
	VkDescriptorPool descriptorPool;

	std::vector<VkWriteDescriptorSet> descriptorWrites;

public:

	DescriptorSet(VkDevice device, VkDescriptorPool descriptorPool);
	~DescriptorSet();

	VkDescriptorSet GetDescriptorSet() { return descriptorSet; }

	void AddDescriptorWrite(uint32_t binding, uint32_t arrayElement, uint32_t descriptorCount,
		VkDescriptorType type, VkDescriptorImageInfo* imageInfo, VkDescriptorBufferInfo* bufferInfo, VkBufferView* bufferView);
	void Init(VkDescriptorSetLayout descriptorSetLayout);


};

