#include "Transform.h"

Transform::Transform() = default;
Transform::~Transform() = default;

// Position
void Transform::setPosition(const glm::vec3& pos) {
    position = pos;
}

void Transform::setPosition(float x, float y, float z) {
    position = glm::vec3(x, y, z);
}

glm::vec3 Transform::getPosition() const {
    return position;
}

void Transform::setRotation(const glm::quat& rot) {
    rotation = rot;
}

void Transform::resetRotation() {
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}

void Transform::setEulerAngles(const glm::vec3& eulerAngles) {
    rotation = glm::quat(eulerAngles);
}

void Transform::setEulerAngles(float pitch, float yaw, float roll) {
    setEulerAngles(glm::vec3(pitch, yaw, roll));
}

glm::vec3 Transform::getEulerAngles() const {
    return glm::eulerAngles(rotation);
}

glm::vec3 Transform::getEulerAnglesDegrees() const {
    return glm::degrees(getEulerAngles());
}

glm::quat Transform::getRotation() const {
    return rotation;
}

// Scale
void Transform::setScale(const glm::vec3& scale) {
    scaling = scale;
}

void Transform::setScale(float scale) {
    scaling = glm::vec3(scale);
}

void Transform::setScale(float x, float y, float z) {
    scaling = glm::vec3(x, y, z);
}

glm::vec3 Transform::getScale() const {
    return scaling;
}

bool Transform::fromOrientation(glm::vec3 origin, glm::vec3 xAxis, glm::vec3 yAxis) {
    xAxis = glm::normalize(xAxis);
    yAxis = glm::normalize(yAxis);
    // check if xAxis and yAxis are parallel
    if (glm::dot(xAxis, yAxis) > 0.9999f) {
        return false;
    }
    glm::vec3 zAxis = glm::cross(xAxis, yAxis);
    yAxis = glm::cross(zAxis, xAxis);
    glm::mat3 rotationMatrix = glm::mat3(xAxis, yAxis, zAxis);
    setRotationMatrix(rotationMatrix);
    setPosition(origin);
    setScale(1.0f);
    return true;
}

glm::mat4 Transform::getMatrix() const {
    glm::mat4 identity = glm::mat4(1.0f);
    glm::mat4 translationMatrix = glm::translate(identity, position);
    glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
    glm::mat4 scaleMatrix = glm::scale(identity, scaling);
    return translationMatrix * rotationMatrix * scaleMatrix;
}

glm::mat4 Transform::getInverseMatrix() const {
    return glm::inverse(getMatrix());
}

void Transform::setMatrix(const glm::mat4& matrix) {
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(matrix, scaling, rotation, position, skew, perspective);
    //rotation = glm::conjugate(rotation);
}

glm::mat3 Transform::getRotationMatrix() const {
    return glm::mat3_cast(rotation);
}

glm::mat4 Transform::getRotationMatrix4() const {
    return glm::mat4_cast(rotation);
}

void Transform::setRotationMatrix(const glm::mat3& rotationMatrix) {
    rotation = glm::quat(rotationMatrix);
}

Transform Transform::getInverse() const {
    Transform newTrans;
    newTrans.setMatrix(getInverseMatrix());
    return newTrans;
}

Transform Transform::operator*(const Transform& other) const {
    Transform newTrans;
    newTrans.setMatrix(getMatrix() * other.getMatrix());
    return newTrans;
}

Transform& Transform::translate(const glm::vec3& translation) {
    position += translation;
    return *this;
}

Transform& Transform::translateLocal(const glm::vec3& translation) {
    position += glm::mat3_cast(rotation) * translation;
    return *this;
}

Transform& Transform::rotate(const glm::quat& rotation) {
    this->rotation = glm::normalize(rotation * this->rotation);
    return *this;
}

Transform& Transform::rotate(const glm::vec3& eulerAngles) {
    rotate(glm::quat(eulerAngles));
    return *this;
}

Transform& Transform::scaleLocal(const glm::vec3& scale) {
    this->scaling *= scale;
    return *this;
}

Transform& Transform::scaleLocal(float scale) {
    this->scaling *= scale;
    return *this;
}

Transform& Transform::scaleLocal(float x, float y, float z) {
    this->scaling *= glm::vec3(x, y, z);
    return *this;
}

void Transform::apply(glm::vec3& pos) {
    pos = glm::vec3(getMatrix() * glm::vec4(pos, 1.0f));
}

Transform& Transform::rotate(const glm::vec3& from, const glm::vec3& to) {
    glm::vec3 fromNorm = glm::normalize(from);
    glm::vec3 toNorm = glm::normalize(to);
    glm::vec3 axis = glm::cross(fromNorm, toNorm);
    float dotVal = glm::dot(fromNorm, toNorm);
    
    if (glm::abs(dotVal) > 0.9999f) {
        // Vectors are parallel, return identity rotation
        return *this;
    }
    
    float angle = glm::acos(glm::clamp(dotVal, -1.0f, 1.0f));
    return rotate(glm::angleAxis(angle, glm::normalize(axis)));
}

Transform& Transform::rotate(const glm::vec3& axis, float angle) {
    return rotate(glm::angleAxis(angle, glm::normalize(axis)));
}

Transform& Transform::transform(const Transform& other) {
    position += other.position;
    rotation = glm::normalize(rotation * other.rotation);
    scaling *= other.scaling;
    return *this;
}

Transform& Transform::inverse() {
    setMatrix(getInverseMatrix());
    return *this;
}

glm::vec3 Transform::xdir() const {
    return glm::normalize(glm::mat3_cast(rotation) * glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 Transform::ydir() const {
    return glm::normalize(glm::mat3_cast(rotation) * glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Transform::zdir() const {
    return glm::normalize(glm::mat3_cast(rotation) * glm::vec3(0.0f, 0.0f, 1.0f));
}

void Transform::lookAt(const glm::vec3& target, const glm::vec3& up) {
    glm::mat4 lookAtMatrix = glm::lookAt(position, target, up);
    setMatrix(lookAtMatrix);
}

void Transform::reset() {
    position = glm::vec3(0.0f);
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    scaling = glm::vec3(1.0f);
}

