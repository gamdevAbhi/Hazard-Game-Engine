#ifndef ENGINE_WINDOW_HPP
#define ENGINE_WINDOW_HPP
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <windows.h>
#include <iostream>

namespace Engine
{
    // engine window class for creating and displaying window
    class Window final
    {
    public:
        inline static const int screen_width = GetSystemMetrics(SM_CXSCREEN);
        inline static const int screen_height = GetSystemMetrics(SM_CYSCREEN);
        glm::ivec2 getSize();
        void makeWindowClose();
    private:
        Window(const char* title, int width, int height);
        void clearWindow();
        void updateWindow();
        void close();
        bool shouldClose();
        void setClearColor(glm::vec3 color);
        GLFWwindow* glfwWindow;
        glm::vec3 clearColor;
        inline static bool onFocus = false;
        static void checkFocus(GLFWwindow* window, int message);
    friend class Camera;
    friend class Input;
    friend class GameLoop;
    };
}

#endif