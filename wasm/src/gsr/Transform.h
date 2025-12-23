#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/matrix_decompose.hpp>

class Transform {
public:
    Transform();
    ~Transform();

    // Position
    void setPosition(const glm::vec3& pos);
    void setPosition(float x, float y, float z);
    glm::vec3 getPosition() const;

    // Rotation
    void setRotation(const glm::quat& rot);
    void resetRotation();
    void setEulerAngles(const glm::vec3& eulerAngles);
    void setEulerAngles(float pitch, float yaw, float roll);
    glm::vec3 getEulerAngles() const;
    glm::vec3 getEulerAnglesDegrees() const;
    glm::quat getRotation() const;

    // Scale
    void setScale(const glm::vec3& scale);
    void setScale(float scale);
    void setScale(float x, float y, float z);
    glm::vec3 getScale() const;

    // convenience methods
    bool fromOrientation(glm::vec3 origin, glm::vec3 xAxis, glm::vec3 yAxis);

    // Transform Matrix
    glm::mat4 getMatrix() const;
    glm::mat4 getInverseMatrix() const;
    void setMatrix(const glm::mat4& matrix);
    glm::mat3 getRotationMatrix() const;
    glm::mat4 getRotationMatrix4() const;
    void setRotationMatrix(const glm::mat3& rotationMatrix);

    Transform getInverse() const;
    Transform operator*(const Transform& other) const;

    // Transform Operations
    Transform& translate(const glm::vec3& translation);
    Transform& translateLocal(const glm::vec3& translation);
    Transform& rotate(const glm::quat& rotation);
    Transform& rotate(const glm::vec3& eulerAngles);
    Transform& rotate(const glm::vec3& from, const glm::vec3& to);
    Transform& rotate(const glm::vec3& axis, float angle);
    Transform& scaleLocal(const glm::vec3& scale);
    Transform& scaleLocal(float scale);
    Transform& scaleLocal(float x, float y, float z);
    Transform& transform(const Transform& other);
    Transform& inverse();
    void apply(glm::vec3& pos);

    // Direction Vectors
    glm::vec3 xdir() const;
    glm::vec3 ydir() const;
    glm::vec3 zdir() const;
    inline glm::vec3 up() const { return ydir(); }
    inline glm::vec3 forward() const { return zdir(); }
    inline glm::vec3 right() const { return xdir(); }

    // Transform Operations
    void lookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
    void reset();

public:
    // Core Transform Components
    glm::vec3 position{0.0f};
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 scaling{1.0f};
};

