
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
	return Resource::loadOBJ(filename, this->m_meshes, this->m_materials);
}

void Actor::draw() const {
	GLFWwindow* window = Game::getInstance()->getMainWindow()->getWindow();
	for (unsigned int i = 0; i < this->m_meshes.size(); i++) {
		const Shader * shader;
		if (this->m_materials.size() > i) {
			this->m_materials[i].use();
			shader = this->m_materials[i].getShader();
		}
		else {
			Material mat;
			mat.use();
			shader = mat.getShader();
		}

		glm::mat4 M = this->m_transform.getTRSMatrix();

		shader->use();
		shader->setUniformValue("u_M", M);
		shader->setUniformValue("u_iTM", glm::mat3(glm::transpose(glm::inverse(M))));
		shader->setUniformValue("u_V", Game::getInstance()->getMainCamera()->getViewMatrix());
		shader->setUniformValue("u_P", Game::getInstance()->getMainCamera()->getProjectionMatrix(window));
		this->m_meshes[i]->draw();
	}
}
