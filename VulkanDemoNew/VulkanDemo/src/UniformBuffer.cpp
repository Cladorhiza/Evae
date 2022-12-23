#include "UniformBuffer.h"


UniformBuffer::UniformBuffer(uint32_t bufferSize, VkDevice dev, VkPhysicalDevice physicalDev) 
    :device(dev), physicalDevice(physicalDev)
{
    VkDeviceSize devBufferSize = bufferSize;
    
    VkHelpers::createBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            buffer, bufferMemory);

    vkMapMemory(device, bufferMemory, 0, bufferSize, 0, &bufferMapped);

}

UniformBuffer::~UniformBuffer() {

    vkDestroyBuffer(device, buffer, nullptr);
    vkFreeMemory(device, bufferMemory, nullptr);
}