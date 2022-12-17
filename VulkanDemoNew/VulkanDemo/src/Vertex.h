#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>

struct Vertex {
    virtual VkVertexInputBindingDescription getBindingDescription() = 0;
    virtual std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() = 0;
};