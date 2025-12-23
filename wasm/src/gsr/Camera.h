#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();
    ~Camera() = default;

    // Position and orientation getters
    glm::vec3 getPosition() const;
    glm::vec3 getDirection() const;
    glm::vec3 getUp() const;
    glm::vec3 getRight() const;

    // Matrix getters
    glm::mat4 getView() const;
    glm::mat4 getProjection() const;

    // Camera configuration
    void setAspect(float aspect);
    void setFOV(float fovRadians);
    void lookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
    void setNearFar(float nearClip, float farClip);

    // Update projection matrix
    void updateProjection();

    // Camera matrices
    glm::mat4 view;
    glm::mat4 proj;

    // Camera parameters
    float nearClip = 0.1f;
    float farClip = 100.0f;
    float aspect = 1.0f; // Window aspect ratio
    float fov = 1.0472f; // Field of view in radians (60 degrees = 1.0472 radians)

    float width = 800.0f; // Window width in pixels
    float height = 600.0f; // Window height in pixels

    // Static main camera
    static Camera* main;
};

