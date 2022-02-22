
#include <Game/Game.hpp>

#include <Engine/Window.hpp>
#include <Engine/Shader.hpp>
#include <Engine/Camera.hpp>
#include <Engine/ResourceLoader.hpp>
#include <Game/Actor.hpp>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>

using namespace std;

Actor::Actor(Map* map) : map(map) {}

Actor::Actor(Map* map, const char* filename) : map(map) {
	if (!loadOBJ(filename)) {
		cout << "Failed to load " << filename << endl;
	}
}

bool Actor::loadOBJ(const char* filename) {
	return Resource::LoadOBJ(filename, this->m_meshes, this->m_materials);
}

void Actor::draw() const {
	GLFWwindow* window = Game::getInstance()->getMainWindow()->GetWindow();
	for (unsigned int i = 0; i < this->m_meshes.size(); i++) {
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

		shader->Use();
		shader->SetUniformValue("u_M", M);
		shader->SetUniformValue("u_iTM", glm::mat3(glm::transpose(glm::inverse(M))));
		shader->SetUniformValue("u_V", Game::getInstance()->getMainCamera()->GetViewMatrix());
		shader->SetUniformValue("u_P", Game::getInstance()->getMainCamera()->GetProjectionMatrix(window));
		this->m_meshes[i]->Draw();
	}
}
