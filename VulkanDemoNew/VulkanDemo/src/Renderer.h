#pragma once

//project files
#include "VkHelpers.h"
#include "InputManager.h"
#include "Texture2D.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "Camera.h"
#include "Utilities.h"
#include "GraphicsPipeline.h"
#include "SwapChain.h"
#include "CommandPool.h"
#include "Instance.h"
#include "Device.h"
#include "Surface.h"
#include "RenderPass.h"
#include "PhysicalDevice.h"
#include "DescriptorSetLayout.h"
#include "DescriptorPool.h"
#include "DescriptorSet.h"
#include "Sprite.h"

//third party
#include "tinyobj/tiny_obj_loader.h"
#include "vma/vk_mem_alloc.h"
#include "stb_image/stb_image.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//std lib
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <optional>
#include <set>
#include <cstdint> 
#include <limits> 
#include <algorithm> 
#include <fstream>
#include <array>
#include <memory>

class Renderer {

private:

    const uint32_t WIDTH = 1280;
    const uint32_t HEIGHT = 720;
    const int MAX_FRAMES_IN_FLIGHT = 2;
    const uint32_t COMMAND_BUFFER_ID_GRAPHICS_RENDER = 0;
    const uint32_t COMMAND_BUFFER_ID_SPRITES_RENDER = 1;
    const std::vector<uint32_t> RECT_INDICES{ 1,2,0,3,0,2 };

    const std::array<std::string, 4> spriteTextures{
        "res/textures/bruno.png",
        "res/textures/theboys.jpg",
        "res/textures/grass.png",
        "res/textures/viking_room.png"
    };


    Vertex v1{ glm::vec3(0.f, 0.f, 0.f),glm::vec3(1.f, 1.f, 1.f), glm::vec2(0.f, 1.f) };
    Vertex v2{ glm::vec3(1.f, 0.f, 0.f),glm::vec3(1.f, 1.f, 1.f), glm::vec2(1.f, 1.f) };
    Vertex v3{ glm::vec3(1.f, 1.f, 0.f),glm::vec3(1.f, 1.f, 1.f), glm::vec2(1.f, 0.f) };
    Vertex v4{ glm::vec3(0.f, 1.f, 0.f),glm::vec3(1.f, 1.f, 1.f), glm::vec2(0.f, 0.f) };

    const std::vector<Vertex> rectVerts{
        v1,
        v2,
        v3,
        v4
    };


    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif


    //application specific
    GLFWwindow* window;
    VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;

    //sync objects
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    //important to maintain order of instance -> device/surface -> the rest
    std::unique_ptr<Instance> instance;
    std::unique_ptr<Surface> surface;
    std::unique_ptr<PhysicalDevice> physicalDevice;
    std::unique_ptr<Device> device;
    std::unique_ptr<RenderPass> renderPass;
    std::unique_ptr<SwapChain> swapChain;
    std::unique_ptr<GraphicsPipeline> graphicsPipeline;
    std::unique_ptr<GraphicsPipeline> graphicsPipeline2;
    std::unique_ptr<DescriptorSetLayout> descriptorSetLayout;
    std::unique_ptr<CommandPool> commandPool;
    std::unique_ptr<DescriptorPool> descriptorPool;
    std::vector<std::unique_ptr<DescriptorSet>> descriptorSets;

    std::vector<std::unique_ptr<Sprite>> sprites;

    bool framebufferResized = false;
    uint32_t currentFrame = 0;


    void createSyncObjects();
    void recreateSwapChain();
    void updateUniformBuffer(uint32_t currentImage, float deltaTime);
    void drawFrame();
    void recordSpriteCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void createGraphicsPipeline();
    void setupDebugMessenger();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    std::vector<const char*> getRequiredExtensions();
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    void InitWindow();
    void InitVulkan();
    void createDescriptorPool();
    void createDescriptorSetLayout();
    void AddSprite(std::string texPath);

public:

    void Init();
    void Render();
    ~Renderer();

};