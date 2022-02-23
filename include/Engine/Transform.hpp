#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform {
private:
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;

public:
    /**
     * @brief Construct a new Transform object
     * 
     * @param position Position of the objet
     * @param rotation Rotation of the objet
     * @param scale Scale of the objet
     */
    Transform(const glm::vec3& position = glm::vec3(0.0f), const glm::quat& rotation = glm::quat(glm::vec3(0.0f)), const glm::vec3& scale = glm::vec3(1.0f));
    
    /**
     * @brief Get the Position of the object
     * 
     * @return glm::vec3 
     */
    glm::vec3 getPosition() const;

    /**
     * @brief Set the Position of the object
     * 
     * @param position 
     */
    void setPosition(const glm::vec3& position);

    /**
     * @brief Translate the object
     * 
     * @param translation 
     */
    void translate(const glm::vec3& translation);
    
    /**
     * @brief Get the Rotation of the objet as a Quaternion
     * 
     * @return glm::quat 
     */
    glm::quat getRotation() const;

    /**
     * @brief Get the Euleur Angles of the object
     * 
     * @return glm::vec3 
     */
    glm::vec3 getEuleurAngles() const;

    /**
     * @brief Set the Rotation of the objet using a Quaternion
     * 
     * @param rotation 
     */
    void setRotation(const glm::quat& rotation);

    /**
     * @brief Set the Euler Angle of the object
     * 
     * @param rotation 
     */
    void setEulerAngle(const glm::vec3& rotation);

    /**
     * @brief Rotate the objet on each axis (in radians)
     * 
     * @param rotation 
     */
    void rotate(const glm::vec3& rotation);

    /**
     * @brief Rotate the objet using a Quaternion
     * 
     * @param rotation 
     */
    void rotate(const glm::quat& rotation);

    /**
     * @brief Get the Scale of the object
     * 
     * @return glm::vec3 
     */
    glm::vec3 getScale() const;

    /**
     * @brief Set the Scale of the object
     * 
     * @param scale 
     */
    void setScale(const glm::vec3& scale);
    
    /**
     * @brief Multiply the current scale by each axis
     * 
     * @param scale 
     */
    void scaleMult(const glm::vec3& scale);

    /**
     * @brief Increase the current scale by each axis
     * 
     * @param scale 
     */
    void scaleAdd(const glm::vec3& scale);

    /**
     * @brief Return the TRS (Translate, Rotation, Scale) Matrix of the Transform
     * 
     * @return mat4 
     */
    glm::mat4 getTRSMatrix() const;
};
