#pragma once

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum MovementDirection {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
};

struct OpenGlCamera {
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    float lastX = 400.0f, lastY = 300.0f, yaw = -90.0f, pitch = 0.0f, fov = 45.0f;
    bool firstMouse = true;

    void attach(GLFWwindow* window) {
        glfwSetWindowUserPointer(window, this);
    }

    void moveCamera(MovementDirection direction, float deltaTime) {
        const float cameraSpeed = 5.0f * deltaTime;
        if (direction == MovementDirection::MOVE_UP) {
            cameraPos += cameraSpeed * cameraFront;
        }
        if (direction == MovementDirection::MOVE_DOWN) {
            cameraPos -= cameraSpeed * cameraFront;
        }
        if (direction == MovementDirection::MOVE_LEFT) {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        if (direction == MovementDirection::MOVE_RIGHT) {
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