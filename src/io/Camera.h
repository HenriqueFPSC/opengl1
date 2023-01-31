///@formatter:off
#ifndef OPENGL1_CAMERA_H
#define OPENGL1_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraDirection {
    NONE = 0,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
    static const float MIN_FOV;
    static const float MAX_FOV;
    void updateCameraVectors();

    glm::vec3 cameraPos;

    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;

    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float speed;
    // float sens;
    float fov;
public:
    Camera(glm::vec3 position);

    void updateCameraDirection(double dx, double dy);
    void updateCameraPosition(CameraDirection direction, double dt);
    void updateCameraFOV(double dy);

    glm::mat4 getViewMatrix() const;
    float getFOV() const;
};


#endif //OPENGL1_CAMERA_H
