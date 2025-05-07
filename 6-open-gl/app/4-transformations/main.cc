#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "config.hpp"
#include "shader.h"
#include "texture.h"
#include "env.h"

// GLFW error callback
void glfw_error_callback(int error, const char *description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

static float mixValue = 0.2f;

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        mixValue += 0.001f;
        if (mixValue >= 1.0f) {
            mixValue = 1.0f;
        }
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        mixValue -= 0.001f;
        if (mixValue <= 0.0f) {
            mixValue = 0.0f;
        }
    }
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

    // Square vertices
    float vertices[] = {
            // positions          // colors           // boxTexture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };

    unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
    };

    OpenGlTexture boxTexture{TexturePath.c_str(), TextureType::JPG};
    stbi_set_flip_vertically_on_load(true);
    OpenGlTexture faceTexture{SecondTexturePath.c_str(), TextureType::PNG};
    stbi_set_flip_vertically_on_load(false);

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

    OpenGlProgram shaderProgram{VertexShaderPath.c_str(), FragmentShaderPath.c_str()};

    // Generate Vertex Array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Linking Vertex Attributes
    // Linking position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Linking color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // Linking boxTexture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);

    // Wireframe mode
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Normal mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    shaderProgram.use();
    shaderProgram.setInt("texture1", 0);
    shaderProgram.setInt("texture2", 1);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // Start ImGui frame
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Clear background
        glClear(GL_COLOR_BUFFER_BIT);

//        float timeValue = glfwGetTime();
//        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
//        int vertexColorLocation = glGetUniformLocation(shaderProgram.id, "ourColor");

        boxTexture.use(GL_TEXTURE0);
        faceTexture.use(GL_TEXTURE1);
        shaderProgram.use();
        shaderProgram.setFloat("mixValue", mixValue);
//        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

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
