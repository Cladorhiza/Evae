#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace VkHelpers
{

	VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);
	VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void copyBuffer(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	VkCommandBuffer beginSingleTimeCommands(VkDevice device, VkCommandPool commandPool);
	void endSingleTimeCommands(VkDevice device, VkCommandPool commandPool, VkQueue Queue, VkCommandBuffer commandBuffer);
	void createBuffer(	VkDevice device, 
						VkPhysicalDevice physicalDevice, 
						VkDeviceSize size, 
						VkBufferUsageFlags usage, 
						VkMemoryPropertyFlags properties, 
						VkBuffer& buffer, 
						VkDeviceMemory& bufferMemory);
	VkExtent2D chooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities);
	void createImage(	VkDevice device, 
						VkPhysicalDevice physicalDevice, 
						uint32_t width, 
						uint32_t height, 
						VkFormat format, 
						VkImageTiling tiling, 
						VkImageUsageFlags usage, 
						VkMemoryPropertyFlags properties, 
						VkImage& image, 
						VkDeviceMemory& imageMemory);
	void transitionImageLayout(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void copyBufferToImage(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
};

