#pragma once
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "VkHelpers.h"

#include <stdexcept>
#include <array>
class SwapChain
{
private:
    VkDevice device;
    VkPhysicalDevice physicalDevice;
    GLFWwindow* window;
    VkSurfaceKHR surface;
    VkRenderPass renderPass;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> images;
    VkFormat imageFormat;
    VkExtent2D extent;
    std::vector<VkImageView> imageViews;
    std::vector<VkFramebuffer> framebuffers;
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;


public:

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    SwapChain(VkDevice dev, VkPhysicalDevice physicalDevice, GLFWwindow* window, VkSurfaceKHR surface, VkRenderPass renderPass);
    ~SwapChain();

    //Getters
    VkSwapchainKHR GetSwapChain() { return swapChain; }
    std::vector<VkImage>& GetImages() { return images; }
    VkFormat GetImageFormat() { return imageFormat; }
    VkExtent2D GetExtent() { return extent; }
    std::vector<VkImageView>& GetImageViews() { return imageViews; }
    std::vector<VkFramebuffer>& GetFrameBuffers() { return framebuffers; }

    void createSwapChain();
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    void createImageViews();
    void createFramebuffers();
    void createDepthResources();
};

