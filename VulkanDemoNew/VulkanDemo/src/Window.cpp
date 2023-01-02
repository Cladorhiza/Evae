#include "Window.h"


void Window::Init(uint32_t w, uint32_t h) {

    width = w;
    height = h;

    if (glfwInit() == GLFW_FALSE) {
        std::cout << "glfw failed to initialize." << std::endl;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);


    window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}

void Window::FramebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
    app->width = width;
    app->height = height;
    while (app->width == 0) {
        glfwWaitEvents();
        app->width = width;
        app->height = height;
    }
}

void Window::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    app->scrollYAxisOffset += yoffset;
}

void Window::PollEvents() {
    glfwPollEvents();
}

std::vector<const char*> Window::GetRequiredExtensions() {

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    return extensions;
}

Window::~Window() {

    glfwDestroyWindow(window);
    glfwTerminate();
}