#include <iostream>
#include <thread>

#define STB_IMAGE_IMPLEMENTATION

#include "stb/stb_image.h"

#include "config.hpp"
#include "shader.h"
#include "camera.h"
#include "env.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLFW error callback
void glfw_error_callback(int error, const char *description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}
int windowWidth = 800, windowHeight = 600;

static float deltaTime = 0.0f;

OpenGlCamera camera{glm::vec3(0.0f, 0.0f, 3.0f)};
float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool firstMouse = true;

const int targetFPS = 60;
const auto targetFrameTime = std::chrono::milliseconds(1000 / targetFPS);

void mouse_callback(GLFWwindow *window, double _xpos, double _ypos) {
    float xpos = static_cast<float>(_xpos);
    float ypos = static_cast<float>(_ypos);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.processScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.moveCamera(MovementDirection::MOVE_UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.moveCamera(MovementDirection::MOVE_DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.moveCamera(MovementDirection::MOVE_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.moveCamera(MovementDirection::MOVE_RIGHT, deltaTime);
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
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

    // Create GLFW window
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "Minimal ImGui Example", nullptr, nullptr);
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

    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    OpenGlProgram shaderProgram{VertexShaderPath.c_str(), FragmentShaderPath.c_str()};
    OpenGlProgram lightSourceShaderProgram{VertexShaderPath.c_str(), LightSourceFragmentShaderPath.c_str()};

    unsigned int qubeVAO;
    glGenVertexArrays(1, &qubeVAO);
    glBindVertexArray(qubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3* sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int  lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);

    glm::vec3 cubePositions = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 lightPosition = glm::vec3(1.2f, 1.0f, 2.0f);

    shaderProgram.use();
    shaderProgram.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    shaderProgram.setVec3("lightColor",  1.0f, 1.0f, 1.0f);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    auto lastFrameTime = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {
        auto frameStart = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float>(frameStart - lastFrameTime).count();
        lastFrameTime = frameStart;

        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Clear background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();

        glm::mat4 view = camera.getView();
        glm::mat4 projection = camera.getProjection(windowWidth, windowHeight);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions);
        shaderProgram.setMat4("view", glm::value_ptr(view));
        shaderProgram.setMat4("projection", glm::value_ptr(projection));
        shaderProgram.setMat4("model", glm::value_ptr(model));
        shaderProgram.setVec3("lightPos", lightPosition);
        shaderProgram.setVec3("viewPos", camera.position);

        glBindVertexArray(qubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);


        double glfwTime = glfwGetTime();
        lightPosition.x = cos(glfwTime) * 2.0f;
        lightPosition.z = sin(glfwTime) * 2.0f;

        model = glm::translate(model, lightPosition);
        model = glm::scale(model, glm::vec3(0.2f));
        lightSourceShaderProgram.use();
        lightSourceShaderProgram.setMat4("view", glm::value_ptr(view));
        lightSourceShaderProgram.setMat4("projection", glm::value_ptr(projection));
        lightSourceShaderProgram.setMat4("model", glm::value_ptr(model));
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // FPS cap
        auto frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> frameDuration = frameEnd - frameStart;
        if (frameDuration < targetFrameTime) {
            std::this_thread::sleep_for(targetFrameTime - frameDuration);
        }
    }

    // Cleanup
    glDeleteVertexArrays(1, &qubeVAO);
    glDeleteBuffers(1, &VBO);

    shaderProgram.clean();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
