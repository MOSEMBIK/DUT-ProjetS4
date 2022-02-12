#pragma once

#include <Engine/Shader.hpp>

class Light {
protected:
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 specular;
public:
    /**
     * @brief Construct a new Light object
     * 
     * @param ambient 
     * @param diffuse 
     * @param specular 
     */
    Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    /**
     * @brief Set the Ambient color of the light
     * 
     * @param ambientColor 
     */
    inline void SetAmbient(glm::vec3 ambientColor) { this->m_ambient = glm::vec3(ambientColor); }

    /**
     * @brief Get the Ambient color of the light
     * 
     * @return glm::vec3 
     */
    inline glm::vec3 GetAmbient() { return m_ambient; }

    /**
     * @brief Set the Diffuse color of the light
     * 
     * @param diffuseColor 
     */
    inline void SetDiffuse(glm::vec3 diffuseColor) { this->m_diffuse = glm::vec3(diffuseColor); }

    /**
     * @brief Get the Diffuse color of the light
     * 
     * @return glm::vec3 
     */
    inline glm::vec3 GetDiffuse() { return m_diffuse; }

    /**
     * @brief Set the Specular color of the light
     * 
     * @param specularColor 
     */
    inline void SetSpecular(glm::vec3 specularColor) { this->specular = glm::vec3(specularColor); }

    /**
     * @brief Get the Specular color of the light
     * 
     * @return glm::vec3 
     */
    inline glm::vec3 GetSpecular() { return specular; }

    /**
     * @brief Send the light information to the shader
     * 
     * @param shader 
     */
    virtual void SendToShader(const Shader& shader) = 0;
};

class DirectionalLight : public Light{
protected:
    glm::vec3 m_direction;
public:
    /**
     * @brief Construct a new Directional Light object
     * 
     * @param direction 
     * @param ambient 
     * @param diffuse 
     * @param specular 
     */
    DirectionalLight(glm::vec3 direction = glm::vec3(0.0f), glm::vec3 ambient = glm::vec3(0.1f), glm::vec3 diffuse = glm::vec3(1.0f), glm::vec3 specular = glm::vec3(1.0f));

    /**
     * @brief Set the Direction of the light
     * 
     * @param direction 
     */
    inline void SetDirection(glm::vec3 direction) { this->m_direction = glm::vec3(direction); }

    /**
     * @brief Get the Direction of the light
     * 
     * @return glm::vec3 
     */
    inline glm::vec3 GetDirection() { return m_direction; }

    /**
     * @brief Send the light information to the shader
     * 
     * @param shader 
     */
    void SendToShader(const Shader& shader);
};

class PointLight : public Light{
protected:
    bool m_enabled;
    unsigned int m_index;
    float m_range;
    glm::vec3 m_position;

public:
    /**
     * @brief Construct a new Point Light object
     * 
     * @param index
     * @param position 
     * @param ambient 
     * @param diffuse 
     * @param specular 
     */
    PointLight(int index, float range = 10.0f, glm::vec3 position = glm::vec3(0.0f), glm::vec3 ambient = glm::vec3(0.0f), glm::vec3 diffuse = glm::vec3(1.0f), glm::vec3 specular = glm::vec3(1.0f));

    /**
     * @brief Get the Position of the light
     * 
     * @return glm::vec3 
     */
    inline glm::vec3 GetPosition() { return m_position; }

    /**
     * @brief Set the Position of the light
     * 
     * @param position 
     */
    inline void SetPosition(glm::vec3 position) { this->m_position = glm::vec3(position); }

    /**
     * @brief Get the Light Range of the light
     * 
     * @return float 
     */
    inline float GetRange() { return m_range; }

    /**
     * @brief Set the Light Distance object
     * 
     * @param range 
     */
    inline void SetRange(float range) { this->m_range = range; }

    /**
     * @brief Send the light information to the shader
     * 
     * @param shader 
     */
    void SendToShader(const Shader& shader);

    /**
     * @brief Enable this point light
     */
    inline void Enable() { this->m_enabled = true;}

    /**
     * @brief Disable this point light
     */
    inline void Disable() { this->m_enabled = false; }
};
