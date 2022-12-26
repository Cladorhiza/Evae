#include "DescriptorPool.h"


DescriptorPool::DescriptorPool(VkDevice device) 
	:device(device), descriptorPool(VK_NULL_HANDLE)
{
}


void DescriptorPool::AddDescriptorPoolSize(VkDescriptorType type, uint32_t count) {

	for (auto& ps : poolSizes) {
		if (ps.type == type) {
			std::cerr << "WARNING: attempted to add duplicate type to descriptor pool sizes! updating count instead.\n";
			ps.descriptorCount = count;
			return;
		}
	}
	poolSizes.emplace_back(type, count);
}
void DescriptorPool::Init(uint32_t maximumSets) {

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = maximumSets;

	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

DescriptorPool::~DescriptorPool() {

	vkDestroyDescriptorPool(device, descriptorPool, nullptr);
}