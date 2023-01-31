
#include "Camera.h"

// -- Static -- //
const float Camera::MIN_FOV = 1.0f;
const float Camera::MAX_FOV = 45.0f;

// ---- Public Methods ---- //

Camera::Camera(glm::vec3 position) :
        cameraPos(position), worldUp(glm::vec3(0, 1, 0)),
        yaw(-90), pitch(0), speed(2.5), fov(45),
        cameraFront(glm::vec3(0, 0, -1)) {
    updateCameraVectors();
}

void Camera::updateCameraDirection(double dx, double dy) {
    yaw += (float) dx;
    pitch += (float) dy;

    if (pitch > 89.0f)
        pitch = 89.0f;
    else if (pitch < -89.0f)
        pitch = -89.0f;

    updateCameraVectors();
}

void Camera::updateCameraPosition(CameraDirection direction, double dt) {
    float velocity = (float) dt * speed;

    switch (direction) {
        case CameraDirection::FORWARD:
            cameraPos += cameraFront * velocity;
            break;
        case CameraDirection::BACKWARD:
            cameraPos -= cameraFront * velocity;
            break;
        case CameraDirection::RIGHT:
            cameraPos += cameraRight * velocity;
            break;
        case CameraDirection::LEFT:
            cameraPos -= cameraRight * velocity;
            break;
        case CameraDirection::UP:
            cameraPos += cameraUp * velocity;
            break;
        case CameraDirection::DOWN:
            cameraPos -= cameraUp * velocity;
            break;
    }
}

void Camera::updateCameraFOV(double dy) {
    if (fov < MIN_FOV)
        fov = MIN_FOV;
    else if (fov > MAX_FOV)
        fov = MAX_FOV;
    else
        fov -= (float) dy;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

// ---- Private Methods ---- //

void Camera::updateCameraVectors() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);

    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

float Camera::getFOV() const {
    return fov;
}
