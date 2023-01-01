#pragma once


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>


class Window
{
private:
	GLFWwindow* window;

	//whether or not frame buffer rezised this frame
	bool framebufferResized;
	
	//in pixels
	int width;
	int height;
	
	
	static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

public:

	~Window();

	GLFWwindow* GetWindow() { return window; }
	int GetWidth() { return width; }
	int GetHeight() { return height; }

	void Init(uint32_t width, uint32_t height);
	static std::vector<const char*> GetRequiredExtensions();

};

