#include "DescriptorSet.h"


DescriptorSet::DescriptorSet(VkDevice device, VkDescriptorPool descriptorPool)
	:device(device), descriptorPool(descriptorPool), descriptorSet(VK_NULL_HANDLE)
{
}

void DescriptorSet::AddDescriptorWrite(uint32_t binding, uint32_t arrayElement, uint32_t descriptorCount,
										VkDescriptorType type, VkDescriptorImageInfo* imageInfo, VkDescriptorBufferInfo* bufferInfo, VkBufferView* bufferView) {

	for (const auto& descWrite : descriptorWrites) {
		if (descWrite.dstBinding == binding) {
			std::cerr << "WARNING: adding binding to descriptor set that already exists!\n";
		}
	}

	descriptorWrites.emplace_back(
		VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		nullptr,
		descriptorSet,
		binding,
		arrayElement,
		descriptorCount,
		type,
		imageInfo,
		bufferInfo,
		bufferView
	);
}
void DescriptorSet::Init(VkDescriptorSetLayout descriptorSetLayout) {

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &descriptorSetLayout;

    if (vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

	for (auto& descWrite : descriptorWrites) {
		descWrite.dstSet = descriptorSet;
	}

    vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

DescriptorSet::~DescriptorSet() {


}