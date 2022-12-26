#include "DescriptorSetLayout.h"

DescriptorSetLayout::DescriptorSetLayout(VkDevice device) 
    :device(device)
{
}

void DescriptorSetLayout::AddBinding(uint32_t bindingIndex, VkDescriptorType descriptorType, uint32_t count, VkShaderStageFlagBits stageFlags) {
    for (const auto& bindin : bindings) {
        if (bindin.binding == bindingIndex) {
            std::cerr << "WARNING: bindind Index already in descriptor layout!\n";
        }
    }
    bindings.emplace_back(bindingIndex, descriptorType, count, stageFlags, nullptr);
}


void DescriptorSetLayout::Init() {

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }


}

DescriptorSetLayout::~DescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

}

