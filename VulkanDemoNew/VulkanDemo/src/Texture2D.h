#pragma once
#include "stb_image/stb_image.h"
#include "VkHelpers.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>

class Texture2D
{
private:
	VkImage Image;
	VkImageView ImageView;
	VkDeviceMemory ImageMemory;
	VkDevice device;
	VkSampler Sampler;

public:

	Texture2D(std::string filePath, VkDevice dev, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue);
	~Texture2D();

	VkImageView GetImageView() { return ImageView; }
	VkSampler GetSampler() { return Sampler; }
};

