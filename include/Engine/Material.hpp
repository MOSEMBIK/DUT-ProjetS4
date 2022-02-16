#pragma once

#include <glm/vec3.hpp>
#include <Engine/Shader.hpp>

class Material {
private:
    glm::vec3 m_ambientColor;
    glm::vec3 m_diffuseColor;
    unsigned int m_diffuseTexture;
    glm::vec3 m_specularColor;
    unsigned int m_specularTexture;
    float m_specularExponent;

    const Shader* m_shader;

    const static unsigned int DIFFUSE_TEXTURE = 1;
    const static unsigned int SPECULAR_TEXTURE = 2;

public:
    /**
     * @brief Construct a new Material object with the Basic Shader
     * 
     */
    Material();

    /**
     * @brief Construct a new Material object
     * 
     */
    Material(const Shader& shader);

    /**
     * @brief Get the Shader
     * 
     * @return const Shader* 
     */
    inline const Shader* GetShader() const { return m_shader; }

    /**
     * @brief Get the Ambient Color of the material
     * 
     * @return glm::vec3 
     */
    inline glm::vec3 GetAmbientColor() const { return m_ambientColor; }

    /**
     * @brief Set the Ambient Color of the material
     * 
     * @param ambientColor 
     */
    inline void SetAmbientColor(const glm::vec3& ambientColor) { m_ambientColor = ambientColor; }

    /**
     * @brief Get the Diffuse Color of the material
     * 
     * @return glm::vec3 
     */
    inline glm::vec3 GetDiffuseColor() const { return m_diffuseColor; }

    /**
     * @brief Set the Diffuse Color of the material
     * 
     * @param diffuseColor 
     */
    inline void SetDiffuseColor(const glm::vec3& diffuseColor) { m_diffuseColor = diffuseColor; }

    /**
     * @brief Get the Diffuse Texture Id
     * 
     * @return unsigned int 
     */
    inline unsigned int GetDiffuseTexture() const { return m_diffuseTexture; }

    /**
     * @brief Set the Diffuse Texture Id
     * 
     * @param diffuseTexture 
     */
    inline void SetDiffuseTexture(int diffuseTexture) { m_diffuseTexture = diffuseTexture; }

    /**
     * @brief Get the Specular Color of the material
     * 
     * @return glm::vec3 
     */
    inline glm::vec3 GetSpecularColor() const { return m_specularColor; }

    /**
     * @brief Set the Specular Color of the material
     * 
     * @param specularColor 
     */
    inline void SetSpecularColor(const glm::vec3& specularColor) { m_specularColor = specularColor; }

    /**
     * @brief Get the Specular Texture Id
     * 
     * @return unsigned int 
     */
    inline unsigned int GetSpecularTexture() const { return m_specularTexture; }

    /**
     * @brief Set the Specular Texture Id
     * 
     * @param specularTexture 
     */
    inline void SetSpecularTexture(int specularTexture) { m_specularTexture = specularTexture; }

    /**
     * @brief Get the Specular Exponent of the material
     * 
     * @return float 
     */
    inline float GetSpecularExponent() const { return m_specularExponent; }

    /**
     * @brief Set the Specular Exponent of the material
     * 
     * @param specularExponent 
     */
    inline void SetSpecularExponent(float specularExponent) { m_specularExponent = specularExponent; }

    void Use() const;
};
