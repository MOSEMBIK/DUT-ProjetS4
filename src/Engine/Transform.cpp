#include <Engine/Transform.hpp>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

Transform::Transform(const vec3& position, const quat& rotation, const vec3& scale)
    : m_position(position), m_rotation(rotation), m_scale(scale) { }

vec3 Transform::getPosition() const
{
    return m_position;
}

void Transform::setPosition(const vec3& position) 
{
    this->m_position = vec3(position);
}

void Transform::translate(const vec3& translation) 
{
    this->m_position += translation;
}

quat Transform::getRotation() const
{
    return m_rotation;
}

vec3 Transform::getEuleurAngles() const
{
    return glm::eulerAngles(m_rotation);
}

void Transform::setRotation(const quat& rotation) 
{
    this->m_rotation = quat(rotation);
}

void Transform::setEulerAngle(const vec3& rotation) 
{
    this->m_rotation = quat(rotation);
}

void Transform::rotate(const vec3& rotation) 
{
    this->m_rotation = this->m_rotation * quat(rotation);
}

void Transform::rotate(const quat& rotation) 
{
    this->m_rotation = this->m_rotation * rotation;
}

vec3 Transform::getScale() const
{
    return m_scale;
}

void Transform::setScale(const vec3& scale) 
{
    this->m_scale = vec3(scale);
}

void Transform::scaleMult(const vec3& scale) 
{
    this->m_scale *= scale;
}

void Transform::scaleAdd(const vec3& scale) 
{
    this->m_scale += scale;
}

mat4 Transform::getTRSMatrix() const
{
    mat4 translate = glm::translate(this->m_position);
    mat4 rotate = glm::toMat4(this->m_rotation);
    mat4 scale = glm::scale(this->m_scale);

    return translate * rotate * scale;
}
