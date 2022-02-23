#pragma once

#include <Engine/Shader.hpp>

class Light {
protected:
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
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
    inline void setAmbient(glm::vec3 ambientColor) { this->m_ambient = glm::vec3(ambientColor); }

    /**
     * @brief Get the Ambient color of the light
     * 
     * @return glm::vec3 
     */
    inline glm::vec3 getAmbient() const { return m_ambient; }

    /**
     * @brief Set the Diffuse color of the light
     * 
     * @param diffuseColor 
     */
    inline void setDiffuse(glm::vec3 diffuseColor) { this->m_diffuse = glm::vec3(diffuseColor); }

    /**
     * @brief Get the Diffuse color of the light
     * 
     * @return glm::vec3 
     */
    inline glm::vec3 getDiffuse() const{ return m_diffuse; }

    /**
     * @brief Set the Specular color of the light
     * 
     * @param specularColor 
     */
    inline void setSpecular(glm::vec3 specularColor) { this->m_specular = glm::vec3(specularColor); }

    /**
     * @brief Get the Specular color of the light
     * 
     * @return glm::vec3 
     */
    inline glm::vec3 setSpecular() const { return m_specular; }

    /**
     * @brief Send the light information to the shader
     * 
     * @param shader 
     */
    virtual void sendToShader(const Shader& shader) const = 0;
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
    inline void setDirection(glm::vec3 direction) { this->m_direction = glm::vec3(direction); }

    /**
     * @brief Get the Direction of the light
     * 
     * @return glm::vec3 
     */
    inline glm::vec3 getDirection() const { return m_direction; }

    /**
     * @brief Send the light information to the shader
     * 
     * @param shader 
     */
    void sendToShader(const Shader& shader) const;
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
     * @brief Reset parameters;
     * 
     */
    void reset();

    /**
     * @brief Get the Position of the light
     * 
     * @return glm::vec3 
     */
    inline glm::vec3 GetPosition() const { return m_position; }

    /**
     * @brief Set the Position of the light
     * 
     * @param position 
     */
    inline void setPosition(glm::vec3 position) { this->m_position = glm::vec3(position); }

    /**
     * @brief Get the Light Range of the light
     * 
     * @return float 
     */
    inline float getRange() const { return m_range; }

    /**
     * @brief Set the Light Distance object
     * 
     * @param range 
     */
    inline void setRange(float range) { this->m_range = range; }

    /**
     * @brief Send the light information to the shader
     * 
     * @param shader 
     */
    void sendToShader(const Shader& shader) const;

    /**
     * @brief Enable this point light
     */
    inline void enable() { this->m_enabled = true;}

    /**
     * @brief Disable this point light
     */
    inline void disable() { this->m_enabled = false; }
};
