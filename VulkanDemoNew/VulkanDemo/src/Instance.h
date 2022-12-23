#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "VkHelpers.h"

#include <iostream>
class Instance
{
private:
	VkInstance instance;


public:

	Instance(	const std::vector<const char*>& validationLayers, 
				const bool enableValidationLayers, 
				const VkDebugUtilsMessengerCreateInfoEXT& messengerCreateInfo,
				const std::vector<const char*>& requiredExtensions);
	~Instance();


	VkInstance GetInstance() { return instance; }

	bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers);
};

