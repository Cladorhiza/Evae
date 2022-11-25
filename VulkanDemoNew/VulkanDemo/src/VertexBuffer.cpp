#include "VertexBuffer.h"










void VertexBuffer::Init(VkDevice dev, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, std::vector<Vertex>& vertices) {
    device = dev;
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VkHelpers::createBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    VkHelpers::createBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer, bufferMemory);

    VkHelpers::copyBuffer(device, commandPool, graphicsQueue, stagingBuffer, buffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void VertexBuffer::Destroy() {

    vkDestroyBuffer(device, buffer, nullptr);
    vkFreeMemory(device, bufferMemory, nullptr);
}