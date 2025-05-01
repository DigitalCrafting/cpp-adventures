#include <iostream>

#include "config.hpp"
#include "dc_utils.h"

// GLFW error callback
void glfw_error_callback(int error, const char *description) {
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

    // Configure GLFW for OpenGL 4.6 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window
    GLFWwindow *window = glfwCreateWindow(800, 600, "Minimal ImGui Example", nullptr, nullptr);
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

    // Triangle vertices
    float vertices[] = {
        0.0f, 0.5f, 0.0f, // top
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f // bottom left
    };

    unsigned int indices[] = {
        0, 2, 1,
    };

    // Generate Vertex Buffer Object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // Bind VBO to be Array Buffer
    // This also means, following buffer operations will be performed on the bound buffer,
    // In this case: buffer with id '1'
    glBindBuffer(GL_ARRAY_BUFFER, 1);
    /*
     * Options of glBufferData usage:
     * - GL_STREAM_DRAW - data is set once, and used byt the GPU at most few times,
     * - GL_STATIC_DRAW - data is set once and used many times,
     * - GL_DYNAMIC_DRAW - data changes a lot and is used many times.
     * */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /* Vertex shader */
    // Define source
    const char* vertexShaderSource = "#version 330 core\n"
                               "layout (location = 0) in vec3 aPos;\n"
                               "void main()\n"
                               "{\n gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n}\n";

    /* Fragment shader */
    const char* fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n }\n";


    OpenGlProgram shaderProgram{vertexShaderSource, fragmentShaderSource};

    // Linking Vertex Attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Generate Vertex Array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Wireframe mode
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Normal mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Main loop
    while (!glfwWindowShouldClose(window)) {

        // Start ImGui frame
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Clear background
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    shaderProgram.clean();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
