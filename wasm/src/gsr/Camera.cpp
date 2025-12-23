#include "Camera.h"

Camera* Camera::main = nullptr;

Camera::Camera() {
    // Initialize with default perspective projection
    proj = glm::perspective(fov, aspect, nearClip, farClip);
    
    // Initialize with default look-at (looking down -Z axis)
    view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f),  // Eye position
        glm::vec3(0.0f, 0.0f, 0.0f),  // Look at origin
        glm::vec3(0.0f, 1.0f, 0.0f)   // Up vector
    );

    // Set as main camera if none exists
    if (main == nullptr) {
        main = this;
    }
}

glm::vec3 Camera::getPosition() const {
    glm::vec3 zero(0.0f, 0.0f, 0.0f);
    glm::mat4 invView = glm::inverse(view);
    glm::vec4 eye4 = invView * glm::vec4(zero, 1.0f);
    glm::vec3 eye(eye4);
    eye /= eye4.w;
    return eye;
}

glm::vec3 Camera::getDirection() const {
    // Extract forward direction from view matrix (negative Z column)
    return -glm::vec3(view[0][2], view[1][2], view[2][2]);
}

glm::vec3 Camera::getUp() const {
    // Extract up direction from view matrix (Y column)
    return glm::vec3(view[0][1], view[1][1], view[2][1]);
}

glm::vec3 Camera::getRight() const {
    // Extract right direction from view matrix (X column)
    return glm::vec3(view[0][0], view[1][0], view[2][0]);
}

glm::mat4 Camera::getView() const {
    return view;
}

glm::mat4 Camera::getProjection() const {
    return proj;
}

void Camera::setAspect(float aspect) {
    this->aspect = aspect;
    updateProjection();
}

void Camera::setFOV(float fovRadians) {
    this->fov = fovRadians;
    updateProjection();
}

void Camera::lookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
    view = glm::lookAt(eye, center, up);
}

void Camera::setNearFar(float nearClip, float farClip) {
    this->nearClip = nearClip;
    this->farClip = farClip;
    updateProjection();
}

void Camera::updateProjection() {
    proj = glm::perspective(fov, aspect, nearClip, farClip);
}

