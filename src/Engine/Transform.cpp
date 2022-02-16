#include <Engine/Transform.hpp>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

Transform::Transform(const vec3& position, const quat& rotation, const vec3& scale)
    : m_position(position), m_rotation(rotation), m_scale(scale) { }

vec3 Transform::GetPosition() const
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

quat Transform::GetRotation() const
{
    return m_rotation;
}

vec3 Transform::GetEuleurAngles() const
{
    return glm::eulerAngles(m_rotation);
}

void Transform::SetRotation(const quat& rotation) 
{
    this->m_rotation = quat(rotation);
}

void Transform::SetEulerAngle(const vec3& rotation) 
{
    this->m_rotation = quat(rotation);
}

void Transform::Rotate(const vec3& rotation) 
{
    this->m_rotation = this->m_rotation * quat(rotation);
}

void Transform::Rotate(const quat& rotation) 
{
    this->m_rotation = this->m_rotation * rotation;
}

vec3 Transform::GetScale() const
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

mat4 Transform::GetTRSMatrix() const
{
    mat4 translate = glm::translate(this->m_position);
    mat4 rotate = glm::toMat4(this->m_rotation);
    mat4 scale = glm::scale(this->m_scale);

    return rotate * translate * scale;
}
