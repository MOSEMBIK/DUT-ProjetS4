#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

class Transform {
private:
    vec3 m_position;
    quat m_rotation;
    vec3 m_scale;

public:
    Transform(const vec3& position = vec3(0.0f), const quat& rotation = quat(vec3(0.0f)), const vec3& scale = vec3(1.0f));
    vec3 GetPosition();
    void SetPosition(const vec3& position);
    void Translate(const vec3& translation);
    
    quat GetRotation();
    vec3 GetEuleurAngles();
    void SetRotation(const quat& rotation);
    void SetEuleurAngle(const vec3& rotation);
    void Rotate(const vec3& rotation);
    void Rotate(const quat& rotation);

    vec3 GetScale();
    void SetScale(const vec3& scale);
    void ScaleMult(const vec3& scale);
    void ScaleAdd(const vec3& scale);

    mat4 GetTRSMatrix();
};
