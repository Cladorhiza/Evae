#pragma once
#include "Vertex.h"
struct BasicVertex :
    public Vertex
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    BasicVertex() 
        :pos(0.f,0.f,0.f), color(1.0f,1.0f,1.0f), texCoord(0.f,0.f)
    {

    }

    BasicVertex(glm::vec3 pos, glm::vec3 color, glm::vec2 texCoord) 
        :pos(pos), color(color), texCoord(texCoord)
    {

    }

    VkVertexInputBindingDescription getBindingDescription() override{
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(BasicVertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() override {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(BasicVertex, pos);


        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(BasicVertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(BasicVertex, texCoord);

        return attributeDescriptions;
    }
};

