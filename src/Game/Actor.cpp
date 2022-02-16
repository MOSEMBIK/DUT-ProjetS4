
#include <Engine/Window.hpp>
#include <Engine/Shader.hpp>
#include <Engine/Camera.hpp>
#include <Engine/ResourceLoader.hpp>
#include <Game/Actor.hpp>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>

Actor::Actor() {}

bool Actor::getCollision() {
	return this->collision;
}

bool Actor::loadOBJ(const char* filename) {
	return Resource::LoadOBJ(filename, this->m_meshes, this->m_materials);
}

void Actor::draw() {
	GLFWwindow* window = Window::GetMain()->GetWindow();
	for (int i = 0; i < this->m_meshes.size(); i++) {
		const Shader * shader;
		if (this->m_materials.size() > i) {
			this->m_materials[i].Use();
			shader = this->m_materials[i].GetShader();
		}
		else {
			Material mat;
			mat.Use();
			shader = mat.GetShader();
		}
		glm::mat4 M = this->m_transform.GetTRSMatrix();

		shader->SetUniformValue("_M", M);
		shader->SetUniformValue("_iTM", glm::mat3(glm::transpose(glm::inverse(M))));
		shader->SetUniformValue("_V", Camera::GetInstance()->GetViewMatrix());
		shader->SetUniformValue("_P", Camera::GetInstance()->GetProjectionMatrix(window));
		this->m_meshes[i].Draw();
	}
}
