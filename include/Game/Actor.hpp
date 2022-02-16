#pragma once

#include <Engine/Mesh.hpp>
#include <Engine/Material.hpp>
#include <Engine/Transform.hpp>
#include <Engine/ResourceLoader.hpp>

class Actor
{
protected:
	std::vector<Mesh> m_meshes;
	std::vector<Material> m_materials;
	bool collision;
	Transform m_transform;

public:
	Actor();

	/**
	 * @brief Get the object collision
	 * 
	 * @return true 
	 * @return false 
	 */
	bool getCollision();

	/**
	 * @brief 
	 * 
	 * @param filename 
	 * @return true 
	 * @return false 
	 */
	bool loadOBJ(const char* filename);

	/**
	 * @brief Draw the object
	 */
	void draw();

	/**
	 * @brief Get the transform
	 * 
	 * @return Transform 
	 */
	inline Transform& getTransform() { return this->m_transform; }

};
