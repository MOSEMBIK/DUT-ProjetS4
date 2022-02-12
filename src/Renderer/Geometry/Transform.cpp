#include <Renderer/Geometry/Transform.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

Transform::Transform(const vec3& position, const quat& rotation, const vec3& scale)
    : m_position(position), m_rotation(rotation), m_scale(scale) { }

vec3 Transform::GetPosition() 
{
    return m_position;
}

void Transform::SetPosition(const vec3& position) 
{
    this->m_position = vec3(position);
}

void Transform::Translate(const vec3& translation) 
{
    this->m_position += translation;
}

quat Transform::GetRotation() 
{
    return m_rotation;
}

vec3 Transform::GetEuleurAngles() 
{
    return glm::eulerAngles(m_rotation);
}

void Transform::SetRotation(const quat& rotation) 
{
    this->m_rotation = quat(rotation);
}

void Transform::SetEuleurAngle(const vec3& rotation) 
{
    this->m_rotation = quat(rotation);
}

void Transform::Rotate(const vec3& rotation) 
{
    this->m_rotation = quat(rotation) * this->m_rotation;
}

void Transform::Rotate(const quat& rotation) 
{
    this->m_rotation = rotation * this->m_rotation;
}

vec3 Transform::GetScale() 
{
    return m_scale;
}

void Transform::SetScale(const vec3& scale) 
{
    this->m_scale = vec3(scale);
}

void Transform::ScaleMult(const vec3& scale) 
{
    this->m_scale *= scale;
}

void Transform::ScaleAdd(const vec3& scale) 
{
    this->m_scale += scale;
}

mat4 Transform::GetTRSMatrix() 
{
    mat4 translate = glm::translate(this->m_position);
    mat4 rotate = glm::toMat4(this->m_rotation);
    mat4 scale = glm::scale(this->m_scale);

    return translate * rotate * scale;
}
