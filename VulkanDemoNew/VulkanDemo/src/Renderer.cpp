#include "Renderer.h"



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


void Renderer::Init() {
    InitWindow();
    InitVulkan();
}

Renderer::~Renderer() {

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


void Renderer::createSyncObjects() {
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


void Renderer::recreateSwapChain() {
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

void Renderer::updateUniformBuffer(uint32_t currentImage, float deltaTime) {

    Sprite::UniformBufferObject ubo{};
    ubo.view = camera.getViewingMatrix();
    ubo.proj = glm::perspective(glm::radians(45.0f), swapChain->GetExtent().width / (float)swapChain->GetExtent().height, 0.1f, 500.0f);
    ubo.proj[1][1] *= -1;

    for (int i{ 0 }; i < sprites.size(); i++) {
        ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(i+deltaTime, 0.0f, 0.0f));

        for (int j{ 0 }; j < MAX_FRAMES_IN_FLIGHT; j++) {

            UniformBuffer* spriteUbo = sprites[i]->GetUniformBuffers(j);
            memcpy(spriteUbo->GetBufferMappedMemory(), &ubo, sizeof(ubo));
        }
    }
}

void Renderer::drawFrame() {

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

    recordSpriteCommandBuffer(commandPool->GetBuffers(COMMAND_BUFFER_ID_SPRITES_RENDER)[currentFrame], imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    std::vector<VkCommandBuffer> cmdBuffs{commandPool->GetBuffers(COMMAND_BUFFER_ID_SPRITES_RENDER)};
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

void Renderer::recordSpriteCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
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

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->GetPipeline());

    for (int i = 0; i < sprites.size(); i++) {

        VkBuffer vertexBuffers[] = {sprites[i]->GetVertexBuffer()->GetBuffer()};
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffer, sprites[i]->GetIndexBuffer()->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

        VkDescriptorSet tempDescSet = sprites[i]->GetDescriptorSets(currentFrame)->GetDescriptorSet();

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->GetPipelineLayout(), 0, 1, &tempDescSet, 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(RECT_INDICES.size()), 1, 0, 0, 0);



    }
    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void Renderer::createGraphicsPipeline() {

    std::filesystem::path fragFile = "frag.spv";
    std::filesystem::path fragCompletePath = shaderPath/fragFile;
    std::filesystem::path vertFile = "vert.spv";
    std::filesystem::path vertCompletePath = shaderPath/vertFile;



    graphicsPipeline = std::make_unique<GraphicsPipeline>();
    graphicsPipeline->AddShaderRaw(VK_SHADER_STAGE_VERTEX_BIT, Utilities::readFile(vertCompletePath.string()));
    graphicsPipeline->AddShaderRaw(VK_SHADER_STAGE_FRAGMENT_BIT, Utilities::readFile(fragCompletePath.string()));
    Vertex v;
    graphicsPipeline->Init(device->GetDevice(), renderPass->GetRenderPass(), descriptorSetLayout->GetDescriptorSetLayout(), &v);
}

void Renderer::setupDebugMessenger() {
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(instance->GetInstance(), &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

void Renderer::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

std::vector<const char*> Renderer::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void Renderer::InitWindow() {
    if (glfwInit() == GLFW_FALSE) {
        std::cout << "glfw failed to initialize." << std::endl;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);


    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    inputManager.Init(window);
}

void Renderer::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

void Renderer::InitVulkan() {

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

    createDescriptorPool();
    createSyncObjects();
    
    sprites.reserve(100);

    //add frame rendering command buffers
    commandPool->AddCommandBuffer(COMMAND_BUFFER_ID_GRAPHICS_RENDER, MAX_FRAMES_IN_FLIGHT, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    commandPool->AddCommandBuffer(COMMAND_BUFFER_ID_SPRITES_RENDER, MAX_FRAMES_IN_FLIGHT, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
}

void Renderer::createDescriptorPool() {

    descriptorPool = std::make_unique<DescriptorPool>(device->GetDevice());

    descriptorPool->AddDescriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES_IN_FLIGHT);
    descriptorPool->AddDescriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_FRAMES_IN_FLIGHT);

    descriptorPool->Init(100);
}

void Renderer::createDescriptorSetLayout() {

    descriptorSetLayout = std::make_unique<DescriptorSetLayout>(device->GetDevice());
    descriptorSetLayout->AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT);
    descriptorSetLayout->AddBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);
    descriptorSetLayout->Init();
}

void Renderer::AddSprite(std::string texPath) {

    sprites.emplace_back(std::make_unique<Sprite>(rectVerts, RECT_INDICES, texPath, device->GetDevice(), physicalDevice->GetPhysicalDevice(), device->GetGraphicsQueue(),
        commandPool->GetCommandPool(), descriptorPool->GetDescriptorPool(), static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT), descriptorSetLayout->GetDescriptorSetLayout()));

}

void Renderer::Render() {

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
        inputManager.Poll(window);
            
        //update logic and draw
        camera.Update(elapsed.count(), inputManager);

        updateUniformBuffer(currentFrame, d);

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
        if (inputManager.GetKeyState(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            break;
        }
    }

    vkDeviceWaitIdle(device->GetDevice());
}
