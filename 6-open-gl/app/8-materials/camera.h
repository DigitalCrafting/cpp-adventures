#pragma once

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum MovementDirection {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_FORWARD,
    MOVE_BACKWARD,
    MOVE_LEFT,
    MOVE_RIGHT,
};

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

struct OpenGlCamera {
    glm::vec3 position   = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up    = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    OpenGlCamera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f), float _yaw = YAW, float _pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
    {
        position = _position;
        worldUp = _up;
        yaw = _yaw;
        pitch = _pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    OpenGlCamera(float _posX, float _posY, float _posZ, float _upX, float _upY, float _upZ, float _yaw, float _pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
    {
        position = glm::vec3(_posX, _posY, _posZ);
        worldUp = glm::vec3(_upX, _upY, _upZ);
        yaw = _yaw;
        pitch = _pitch;
        updateCameraVectors();
    }

    void moveCamera(MovementDirection direction, float deltaTime) {
        const float velocity = movementSpeed * deltaTime;
        if (direction == MovementDirection::MOVE_UP) {
            position += up * velocity;
        }
        if (direction == MovementDirection::MOVE_DOWN) {
            position -= up * velocity;
        }
        if (direction == MovementDirection::MOVE_FORWARD) {
            position += front * velocity;
        }
        if (direction == MovementDirection::MOVE_BACKWARD) {
            position -= front * velocity;
        }
        if (direction == MovementDirection::MOVE_LEFT) {
            position -= right * velocity;
        }
        if (direction == MovementDirection::MOVE_RIGHT) {
            position += right * velocity;
        }
    }

    void processMouseMovement(double xOffset, double yOffset, GLboolean constrainPitch = true) {
        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;

        yaw += xOffset;
        pitch += yOffset;

        if (constrainPitch) {
            if (pitch > 89.0f) {
                pitch = 89.0f;
            }
            if (pitch < -89.0f) {
                pitch = -89.0f;
            }
        }

        updateCameraVectors();
    }

    void processScroll(float yOffset) {
        zoom -= yOffset;
        if (zoom < 1.0f) {
            zoom = 1.0f;
        }
        if (zoom > 45.0f) {
            zoom = 45.0f;
        }
    }

    glm::mat4 getView() {
        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 getProjection(int width, int height) {
        return glm::perspective(glm::radians(zoom), (float) width / (float) height, 0.1f, 100.0f);
    }

private:
    void updateCameraVectors() {
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);

        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};