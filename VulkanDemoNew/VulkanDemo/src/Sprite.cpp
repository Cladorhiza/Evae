#include "Sprite.h"

Sprite::Sprite(std::vector<Vertex> vertices, std::vector<uint32_t> indexes, std::string texFilePath, VkDevice device, 
				VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool, 
				VkDescriptorPool descriptorPool, uint32_t frameBufferCount, VkDescriptorSetLayout layout)
	:vertices(vertices), texture(texFilePath, device, physicalDevice, commandPool, graphicsQueue), modelMatrix(1.f), 
	vertexBuffer(device, physicalDevice, commandPool, graphicsQueue, vertices), indexBuffer(device, physicalDevice, commandPool, graphicsQueue, indexes)
{
	

	VkDescriptorImageInfo imageInfo = GetImageInfo();

	VkDescriptorBufferInfo bufferInfo{};
	bufferInfo.offset = 0;
	bufferInfo.range = UBO_MVP_SIZE;
	uniformBuffers.reserve(frameBufferCount);

	for (int i{ 0 }; i < frameBufferCount; i++) {

		uniformBuffers.emplace_back(UBO_MVP_SIZE, device, physicalDevice);
		bufferInfo.buffer = uniformBuffers[i].GetBuffer();
		descriptorSets.emplace_back(device, descriptorPool);
		descriptorSets[i].AddDescriptorWrite(0, 0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nullptr, &bufferInfo, nullptr);
		descriptorSets[i].AddDescriptorWrite(1, 0, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &imageInfo, nullptr, nullptr);
		descriptorSets[i].Init(layout);
	}
}

VkDescriptorImageInfo Sprite::GetImageInfo() {

	VkDescriptorImageInfo imageInfo{};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = texture.GetImageView();
	imageInfo.sampler = texture.GetSampler();

	return imageInfo;
}
Sprite::~Sprite() {

	
}