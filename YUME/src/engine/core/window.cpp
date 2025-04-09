#include "window.h"
#include <iostream>

namespace yume {
    unsigned int WINDOW_WIDTH{ 720 };
    unsigned int WINDOW_HEIGHT{ 720 };

    float WINDOW_WIDTH_SCALE{};
    float WINDOW_HEIGHT_SCALE{};

    GLFWwindow* _window{ nullptr };

    float delta_time{};
    float last_frame{};

    void setWindowSize(unsigned int width, unsigned int height) {
        WINDOW_WIDTH = width;
        WINDOW_HEIGHT = height;
    }

    void initWindow(const std::string& title) {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

        _window = glfwCreateWindow((int)WINDOW_WIDTH, (int)WINDOW_HEIGHT, title.c_str(), nullptr, nullptr);

        WINDOW_WIDTH_SCALE = static_cast<float>(WINDOW_WIDTH) / 720;
        WINDOW_HEIGHT_SCALE = static_cast<float>(WINDOW_HEIGHT) / 720;

        if (_window == nullptr) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        else {
            std::cout << "GLFW window created correctly" << std::endl;
        }

        glfwMakeContextCurrent(_window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD\n";
            return;
        }
        else {
            std::cout << "GLAD initialized correctly\n";
            std::cout << "YUME 0.0.1 started\n";
        }

        glfwSwapInterval(1);
        std::cout << std::flush;
    }

    void updateWindow() {
        auto current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
    }

    GLFWwindow* getWindowPointer() {
        return _window;
    }

    bool isWindowOpen() {
        return !glfwWindowShouldClose(_window);
    }

    void setWindowStatus(bool value) {
        glfwSetWindowShouldClose(_window, !value);
    }

