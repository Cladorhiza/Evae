#include "CommandPool.h"



CommandPool::CommandPool(VkDevice dev, VkPhysicalDevice physicalDev, VkSurfaceKHR surface) {

    device = dev;
    physicalDevice = physicalDev;

    VkHelpers::QueueFamilyIndices queueFamilyIndices = VkHelpers::findQueueFamilies(physicalDevice, surface);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

void CommandPool::AddCommandBuffer(int id, uint32_t count, VkCommandBufferLevel buffLevel) {
    commandBufferMap[id].resize(count);
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = buffLevel;
    allocInfo.commandBufferCount = count;

    if (vkAllocateCommandBuffers(device, &allocInfo, commandBufferMap[id].data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

CommandPool::~CommandPool() {

    vkDestroyCommandPool(device, commandPool, nullptr);
}