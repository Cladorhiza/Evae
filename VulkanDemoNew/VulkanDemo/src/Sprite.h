#pragma once
#include "VkHelpers.h"
#include "Texture2D.h"
#include "Vertex.h"
#include "DescriptorSet.h"
#include "UniformBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
class Sprite
{

private:

	std::vector<Vertex> vertices;

	Texture2D texture;

	glm::mat4 modelMatrix;
	
	std::vector<DescriptorSet> descriptorSets;

	std::vector<UniformBuffer> uniformBuffers;

	VertexBuffer vertexBuffer;

	IndexBuffer indexBuffer;

public:

	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	static constexpr uint32_t UBO_MVP_SIZE = sizeof(UniformBufferObject);

	

	Sprite(std::vector<Vertex> vertices, std::vector<uint32_t> indexes, std::string texFilePath, VkDevice device,
			VkPhysicalDevice physicalDev, VkQueue graphicsQueue, VkCommandPool commandPool, 
			VkDescriptorPool descriptorPool, uint32_t frameBufferCount, VkDescriptorSetLayout layout);
	~Sprite();
	VertexBuffer* GetVertexBuffer() { return &vertexBuffer; }
	IndexBuffer* GetIndexBuffer() { return &indexBuffer; }
	DescriptorSet* GetDescriptorSets(uint32_t index) { return &descriptorSets[index]; }
	UniformBuffer* GetUniformBuffers(uint32_t index) { return &uniformBuffers[index]; }
	std::vector<Vertex>& GetVertices() { return vertices; }
	void SetModelMatrix(glm::mat4 modelMat) { modelMatrix = modelMat; }
	glm::mat4& GetModelMatrix() { return modelMatrix; }
	VkDescriptorImageInfo GetImageInfo();


};
