#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct OpenGlCamera {
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    float lastX = 400.0f, lastY = 300.0f, yaw = -90.0f, pitch = 0.0f, fov = 45.0f;
    bool firstMouse = true;

    void attach(GLFWwindow* window) {
        glfwSetWindowUserPointer(window, this);
    }

    void processInput(GLFWwindow* window, float deltaTime) {
        const float cameraSpeed = 5.0f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cameraPos += cameraSpeed * cameraFront;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cameraPos -= cameraSpeed * cameraFront;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
    }

    void mouseCallback(double xPosIn, double yPosIn) {
        float xPos = static_cast<float>(xPosIn);
        float yPos = static_cast<float>(yPosIn);

        if (firstMouse) {
            lastX = xPos;
            lastY = yPos;
            firstMouse = false;
        }

        float xOffset = xPos - lastX;
        float yOffset = lastY - yPos;
        lastX = xPos;
        lastY = yPos;

        float sensitivity = 0.1f;
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        yaw += xOffset;
        pitch += yOffset;

        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
        if (pitch < -89.0f) {
            pitch = -89.0f;
        }

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw) * cos(glm::radians(pitch)));
        cameraFront = glm::normalize(direction);
    }

    void scrollCallback(double xOffset, double yOffset) {
        fov -= (float) yOffset;
        if (fov < 1.0f) {
            fov = 1.0f;
        }
        if (fov > 45.0f) {
            fov = 45.0f;
        }
    }

    glm::mat4 getView() {
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }
};