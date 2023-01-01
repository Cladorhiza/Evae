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
	//mouse scroll axis
	double scrollYAxisOffset;
	

	
	static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
public:

	~Window();

	GLFWwindow* GetWindow() { return window; }
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	double GetScrollYOffSet() { return scrollYAxisOffset; }
	bool WindowShouldClose() { return glfwWindowShouldClose(window); }
	
	
	void PollEvents();
	void Init(uint32_t width, uint32_t height);
	static std::vector<const char*> GetRequiredExtensions();
};

