#include <iostream>

#include "config.hpp"
#include "imgui.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "../../external/implot/implot.h"

// GLFW error callback
void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main() {
    std::cout << project_name << '\n';
    std::cout << project_version << '\n';

    // Set GLFW error callback
    glfwSetErrorCallback(glfw_error_callback);

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW for OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Minimal ImGui Example", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make OpenGL context current
    glfwMakeContextCurrent(window);

    // Enable V-Sync
    glfwSwapInterval(1);

    // Initialize GLAD
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Print OpenGL version
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // Set up ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Initialize ImGui for GLFW and OpenGL3
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    int   bar_data[11] = {1,2,3,4,5,6,7,8,9,10,11};
    float x_data[1000] = {};
    float y_data[1000] = {};

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll events
        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Example ImGui window
        ImGui::Begin("Hello, ImGui!"); // Create a window
        if (ImPlot::BeginPlot("My Plot")) {
            ImPlot::PlotBars("My Bar Plot", bar_data, 11);
            ImPlot::PlotLine("My Line Plot", x_data, y_data, 1000);
            ImPlot::EndPlot();
        }
        ImGui::Text("This is a minimal ImGui example."); // Text inside the window
        ImGui::End(); // Close the window

        // Render ImGui
        ImGui::Render();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Clear background
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
