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

const uint32_t WIDTH = 1800;
const uint32_t HEIGHT = 1000;
const int MAX_FRAMES_IN_FLIGHT = 2;
const uint32_t COMMAND_BUFFER_ID_GRAPHICS_RENDER = 0;

const std::string MODEL_PATH = "res/models/viking_room.obj";
const std::string TEXTURE_PATH = "res/textures/viking_room.png";

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

constexpr uint32_t UBO_MVP_SIZE = sizeof(UniformBufferObject);

 static bool textured = true;

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

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    }

    return VK_FALSE;
}



VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}




class HelloTriangleApplication {
public:
    void Run() {
        InitWindow();
        InitVulkan();
        MainLoop();
    }

    ~HelloTriangleApplication() {

        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(instance->GetInstance(), debugMessenger, nullptr);
        }
        
        

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(device->GetDevice(), renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(device->GetDevice(), imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(device->GetDevice(), inFlightFences[i], nullptr);
        }
        glfwDestroyWindow(window);
        glfwTerminate();
    }

private:
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
    std::unique_ptr<Texture2D> tex;
    std::unique_ptr<Texture2D> vikingTexture;
    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<IndexBuffer> indexBuffer;
    std::vector<std::unique_ptr<UniformBuffer>> uniformBuffersMVP;
    std::unique_ptr<CommandPool> commandPool;
    std::unique_ptr<DescriptorPool> descriptorPool;
    std::vector<std::unique_ptr<DescriptorSet>> descriptorSets;

    //needs refactoring
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    bool framebufferResized = false;
    uint32_t currentFrame = 0;

    void createSyncObjects() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(device->GetDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(device->GetDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(device->GetDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {

                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
        }
    }


    void recreateSwapChain() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(device->GetDevice());

        swapChain.reset();
        swapChain = std::make_unique<SwapChain>(device->GetDevice(), physicalDevice->GetPhysicalDevice(), window, surface->GetSurface(), renderPass->GetRenderPass());
    }

    void updateUniformBuffer(uint32_t currentImage) {

        UniformBufferObject ubo{};

        ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f));
        ubo.view = Camera::getViewingMatrix();
        ubo.proj = glm::perspective(glm::radians(45.0f), swapChain->GetExtent().width / (float)swapChain->GetExtent().height, 0.1f, 500.0f);
        ubo.proj[1][1] *= -1;
                
        memcpy(uniformBuffersMVP[currentImage]->GetBufferMappedMemory(), &ubo, sizeof(ubo));
    }

    void drawFrame() {

        vkWaitForFences(device->GetDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(device->GetDevice(), swapChain->GetSwapChain(), UINT32_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            recreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }
        
        vkResetFences(device->GetDevice(), 1, &inFlightFences[currentFrame]);

        vkResetCommandBuffer(commandPool->GetBuffers(COMMAND_BUFFER_ID_GRAPHICS_RENDER)[currentFrame], 0);
        recordCommandBuffer(commandPool->GetBuffers(COMMAND_BUFFER_ID_GRAPHICS_RENDER)[currentFrame], imageIndex);

        updateUniformBuffer(currentFrame);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        std::vector<VkCommandBuffer> cmdBuffs{commandPool->GetBuffers(0)};
        submitInfo.pCommandBuffers = &cmdBuffs[currentFrame];

        VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        VkResult result2 = vkQueueSubmit(device->GetGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]);
        if (result2 != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { swapChain->GetSwapChain() };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        presentInfo.pResults = nullptr; // Optional

        result = vkQueuePresentKHR(device->GetPresentQueue(), &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
            framebufferResized = false;
            recreateSwapChain();
        }
        else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass->GetRenderPass();
        renderPassInfo.framebuffer = swapChain->GetFrameBuffers()[imageIndex];

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapChain->GetExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
        clearValues[1].depthStencil = { 1.0f, 0 };
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain->GetExtent().width);
        viewport.height = static_cast<float>(swapChain->GetExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = swapChain->GetExtent();
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        
        if (InputManager::GetKeyToggle(GLFW_KEY_T)){
            textured = !textured;
        }
        if (textured) {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->GetPipeline());
        }
        else {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline2->GetPipeline());
        }

        VkBuffer vertexBuffers[] = { vertexBuffer->GetBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

        VkDescriptorSet tempDescSet = descriptorSets[currentFrame]->GetDescriptorSet();

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->GetPipelineLayout(), 0, 1, &tempDescSet, 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void loadModel() {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;
        int dupeVertexCount = 0;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str())) {
            throw std::runtime_error(warn + err);
        }

        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex{};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = { 1.0f, 1.0f, 1.0f };

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }
                else
                    dupeVertexCount++;

                indices.push_back(uniqueVertices[vertex]);
            }
        }
        std::cout << "Model loader removed " << dupeVertexCount << " duplicate vertices!\n";
    }

    void createGraphicsPipeline() {
        graphicsPipeline = std::make_unique<GraphicsPipeline>();
        graphicsPipeline->AddShaderRaw(VK_SHADER_STAGE_VERTEX_BIT, Utilities::readFile("res/shaders/vert.spv"));
        graphicsPipeline->AddShaderRaw(VK_SHADER_STAGE_FRAGMENT_BIT, Utilities::readFile("res/shaders/frag.spv"));
        Vertex v;
        graphicsPipeline->Init(device->GetDevice(), renderPass->GetRenderPass(), descriptorSetLayout->GetDescriptorSetLayout(), &v);
        graphicsPipeline2 = std::make_unique<GraphicsPipeline>();
        graphicsPipeline2->AddShaderRaw(VK_SHADER_STAGE_VERTEX_BIT, Utilities::readFile("res/shaders/vert2.spv"));
        graphicsPipeline2->AddShaderRaw(VK_SHADER_STAGE_FRAGMENT_BIT, Utilities::readFile("res/shaders/frag2.spv"));
        graphicsPipeline2->Init(device->GetDevice(), renderPass->GetRenderPass(), descriptorSetLayout->GetDescriptorSetLayout(), &v);
    }

    void setupDebugMessenger() {
        if (!enableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        populateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(instance->GetInstance(), &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    std::vector<const char*> getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    void InitWindow() {
        if (glfwInit() == GLFW_FALSE) {
            std::cout << "glfw failed to initialize." << std::endl;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);


        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        InputManager::Init(window);
    }

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }

    void InitVulkan() {

        //create instance
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        populateDebugMessengerCreateInfo(debugCreateInfo);
        std::vector<const char*> requiredExtensions = getRequiredExtensions();
        instance = std::make_unique<Instance>(validationLayers, enableValidationLayers, debugCreateInfo, requiredExtensions);


        setupDebugMessenger();
        
        //create surface
        surface = std::make_unique<Surface>(instance->GetInstance(), window);
        
        //pick physical device
        physicalDevice = std::make_unique<PhysicalDevice>(instance->GetInstance(), surface->GetSurface(), deviceExtensions);
        
        //create logical device
        device = std::make_unique<Device>(physicalDevice->GetPhysicalDevice(), surface->GetSurface(), enableValidationLayers, deviceExtensions, validationLayers);

        //create render pass
        renderPass = std::make_unique<RenderPass>(device->GetDevice(), physicalDevice->GetPhysicalDevice(), surface->GetSurface());


        //create swap chain
        swapChain = std::make_unique<SwapChain>(device->GetDevice(), physicalDevice->GetPhysicalDevice(), window, surface->GetSurface(), renderPass->GetRenderPass());


        createDescriptorSetLayout();
        createGraphicsPipeline();

        //create command pool
        commandPool = std::make_unique<CommandPool>(device->GetDevice(), physicalDevice->GetPhysicalDevice(), surface->GetSurface());


        //add frame rendering command buffers
        commandPool->AddCommandBuffer(COMMAND_BUFFER_ID_GRAPHICS_RENDER, MAX_FRAMES_IN_FLIGHT, VK_COMMAND_BUFFER_LEVEL_PRIMARY);

        
        //create textures
        tex = std::make_unique<Texture2D>("res/textures/grass.png", device->GetDevice(), physicalDevice->GetPhysicalDevice(), commandPool->GetCommandPool(), device->GetGraphicsQueue());
        vikingTexture = std::make_unique<Texture2D>(TEXTURE_PATH, device->GetDevice(), physicalDevice->GetPhysicalDevice(), commandPool->GetCommandPool(), device->GetGraphicsQueue());


        
        loadModel();
        
        //create buffers
        vertexBuffer = std::make_unique<VertexBuffer>(device->GetDevice(), physicalDevice->GetPhysicalDevice(), commandPool->GetCommandPool(), device->GetGraphicsQueue(), vertices);
        indexBuffer = std::make_unique<IndexBuffer>(device->GetDevice(), physicalDevice->GetPhysicalDevice(), commandPool->GetCommandPool(), device->GetGraphicsQueue(), indices);


        //create uniform buffers
        uniformBuffersMVP.resize(MAX_FRAMES_IN_FLIGHT);
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            uniformBuffersMVP[i] = std::make_unique<UniformBuffer>(UBO_MVP_SIZE, device->GetDevice(), physicalDevice->GetPhysicalDevice());
        }


        createDescriptorPool();
        createDescriptorSets();
        createSyncObjects();
    }

    void createDescriptorSets() {

        descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = vikingTexture->GetImageView();
        imageInfo.sampler = vikingTexture->GetSampler();

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);
        
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

            bufferInfo.buffer = uniformBuffersMVP[i]->GetBuffer();

            descriptorSets[i] = std::make_unique<DescriptorSet>(device->GetDevice(), descriptorPool->GetDescriptorPool());

            descriptorSets[i]->AddDescriptorWrite(0,0,1,VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,nullptr,&bufferInfo,nullptr);
            descriptorSets[i]->AddDescriptorWrite(1,0,1,VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &imageInfo, nullptr, nullptr);

            descriptorSets[i]->Init(descriptorSetLayout->GetDescriptorSetLayout());

        }

        /*std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout->GetDescriptorSetLayout());
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool->GetDescriptorPool();
        allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
        if (vkAllocateDescriptorSets(device->GetDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffersMVP[i]->GetBuffer();
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = vikingTexture->GetImageView();
            imageInfo.sampler = vikingTexture->GetSampler();

            std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = descriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = descriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo = &imageInfo;

            vkUpdateDescriptorSets(device->GetDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }*/
    }

    void createDescriptorPool() {

        descriptorPool = std::make_unique<DescriptorPool>(device->GetDevice());

        descriptorPool->AddDescriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES_IN_FLIGHT);
        descriptorPool->AddDescriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_FRAMES_IN_FLIGHT);

        descriptorPool->Init(100);
    }

    void createDescriptorSetLayout() {

        descriptorSetLayout = std::make_unique<DescriptorSetLayout>(device->GetDevice());
        descriptorSetLayout->AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT);
        descriptorSetLayout->AddBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);
        descriptorSetLayout->Init();
    }

    void MainLoop() {

        //delta-time storage
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        std::chrono::duration<float> elapsed;
        
        //used to test how many frames per second
        double d = 0.0;
        int framesCount = 0;

        //main game loop
        while (!glfwWindowShouldClose(window)) {

            start = std::chrono::high_resolution_clock::now();
            
            //poll inputs
            InputManager::Poll(window);
            
            //update logic and draw
            Camera::Update(elapsed.count());
            drawFrame();
            
            
            end = std::chrono::high_resolution_clock::now();
            
            //calc delta time
            elapsed = (end - start);

            //framerate calculation
            d += elapsed.count();
            framesCount++;
            if (d > std::chrono::seconds(1).count()) {
                d -= std::chrono::seconds(1).count();
                std::cout << framesCount << std::endl;
                framesCount = 0;
            }

            //exit program on escape
            if (InputManager::GetKeyState(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                break;
            }
        }

        vkDeviceWaitIdle(device->GetDevice());
    }
};

int main() {
    HelloTriangleApplication app;

    try {
        app.Run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}