#pragma once

//project files
#include "VkHelpers.h"
#include "Texture2D.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
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
#include "Window.h"

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
#include <filesystem>

class Renderer {

private:

    const uint32_t WIDTH = 1280;
    const uint32_t HEIGHT = 720;
    const int MAX_FRAMES_IN_FLIGHT = 2;
    const uint32_t COMMAND_BUFFER_ID_GRAPHICS_RENDER = 0;
    const uint32_t COMMAND_BUFFER_ID_SPRITES_RENDER = 1;
    const std::vector<uint32_t> RECT_INDICES{ 1,2,0,3,0,2 };
    const std::filesystem::path shaderPath = "../VulkanDemo/res/shaders/";

    Vertex v1{ glm::vec3(-0.5f, -0.5f, 0.f),glm::vec3(1.f, 1.f, 1.f), glm::vec2(0.f, 1.f) };
    Vertex v2{ glm::vec3(0.5f, -0.5f, 0.f),glm::vec3(1.f, 1.f, 1.f), glm::vec2(1.f, 1.f) };
    Vertex v3{ glm::vec3(0.5f, 0.5f, 0.f),glm::vec3(1.f, 1.f, 1.f), glm::vec2(1.f, 0.f) };
    Vertex v4{ glm::vec3(-0.5f, 0.5f, 0.f),glm::vec3(1.f, 1.f, 1.f), glm::vec2(0.f, 0.f) };

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
    Window GLFWwindow;
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
    std::unique_ptr<DescriptorSetLayout> descriptorSetLayout;
    std::unique_ptr<CommandPool> commandPool;
    std::unique_ptr<DescriptorPool> descriptorPool;
    std::vector<std::unique_ptr<DescriptorSet>> descriptorSets;

    //rendering loop items
    std::vector<std::unique_ptr<Sprite>> sprites;
    Sprite::UniformBufferObject mvpUbo;

    bool framebufferResized = false;
    uint32_t currentFrame = 0;

    void createSyncObjects();
    void recreateSwapChain();
    void UpdateUniformBuffers();
    void drawFrame();
    void recordSpriteCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void createGraphicsPipeline();
    void setupDebugMessenger();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    std::vector<const char*> getRequiredExtensions();
    void InitVulkan();
    void createDescriptorPool();
    void createDescriptorSetLayout();

public:

    ~Renderer();

    Window& GetWindow() { return GLFWwindow; }
    void SetViewMatrix(glm::mat4 viewMatrix) { mvpUbo.view = viewMatrix; }
    void SetProjMatrix(glm::mat4 projMatrix) { mvpUbo.proj = projMatrix; }

    void Init();
    void Render();
    void AddSprite(std::string texPath);
    void UpdateSpriteModelMatrixes(std::vector<glm::mat4> spriteModelMats);

};