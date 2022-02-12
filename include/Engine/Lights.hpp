#pragma once

#include <Engine/Shader.hpp>

class Light {
protected:
    glm::vec3 ambient;
    glm::vec3 diffuse;
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
    inline void SetAmbient(glm::vec3 ambientColor) { this->ambient = glm::vec3(ambientColor); }

    /**
     * @brief Get the Ambient color of the light
     * 
     * @return glm::vec3 
     */
    inline glm::vec3 GetAmbient() { return ambient; }

    /**
     * @brief Set the Diffuse color of the light
     * 
     * @param diffuseColor 
     */
    inline void SetDiffuse(glm::vec3 diffuseColor) { this->diffuse = glm::vec3(diffuseColor); }

    /**
     * @brief Get the Diffuse color of the light
     * 
     * @return glm::vec3 
     */
    inline glm::vec3 GetDiffuse() { return diffuse; }

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
    glm::vec3 direction;
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
    inline void SetDirection(glm::vec3 direction) { this->direction = glm::vec3(direction); }

    /**
     * @brief Get the Direction of the light
     * 
     * @return glm::vec3 
     */
    inline glm::vec3 GetDirection() { return direction; }

    /**
     * @brief Send the light information to the shader
     * 
     * @param shader 
     */
    void SendToShader(const Shader& shader);
};

class PointLight : public Light{
protected:
    bool enabled;
    unsigned int index;
    float range;
    glm::vec3 position;

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
    inline glm::vec3 GetPosition() { return position; }

    /**
     * @brief Set the Position of the light
     * 
     * @param position 
     */
    inline void SetPosition(glm::vec3 position) { this->position = glm::vec3(position); }

    /**
     * @brief Get the Light Range of the light
     * 
     * @return float 
     */
    inline float GetRange() { return range; }

    /**
     * @brief Set the Light Distance object
     * 
     * @param range 
     */
    inline void SetRange(float range) { this->range = range; }

    /**
     * @brief Send the light information to the shader
     * 
     * @param shader 
     */
    void SendToShader(const Shader& shader);

    /**
     * @brief Enable this point light
     */
    inline void Enable() { enabled = true;}

    /**
     * @brief Disable this point light
     */
    inline void Disable() { enabled = false; }
};
