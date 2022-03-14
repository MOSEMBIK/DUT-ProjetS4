
#include <Game/Map.hpp>
#include <Game/Actor.hpp>
#include <numeric>
#include <algorithm>
#include <iterator> 
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace glm;

Map::Map() : mapMaterial(*Shader::find("Base"))
{
	Resource::loadTexture("assets/map_texture.png", mapTexture);
	Resource::loadTexture("assets/map_texture_specular.png", mapTextureSpecular);
	mapMaterial.setDiffuseTexture(mapTexture->m_id);
	mapMaterial.setSpecularTexture(mapTextureSpecular->m_id);
}

Map::~Map() {

	std::cerr << "Hello";
	for (Actor* actor : actors) {
		if(actor != nullptr)
			delete actor;
	}
	
	for (auto wall : walls) {
		if(wall.second != nullptr)
			delete wall.second;
	}
	std::cerr << "O<O";
}

void Map::generateMap(int size, int wallPercentage) {
	walls.clear();
	if (size % 2 == 0)
		size++;
	this->mapSize = size;;
	int sizeMax = size-1;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			ivec2 pos(i, j);
			/**
			 * @brief Metal Walls
			 */
			if (i == 0 || i == sizeMax || j == 0 || j == sizeMax || (i%2 == 0 && j%2 == 0)) {
				walls[pos] = new Wall(this, Wall::Type::Metal);
				walls[pos]->getTransform().setPosition(vec3(i, 0, j));
			}
			/**
			 * @brief Random Walls
			 */
			else if (rand() % 100 < wallPercentage) {
				walls[pos] = new Wall(this, (rand()%5 == 0) ? Wall::Type::Stone : Wall::Type::Wood);
				walls[pos]->getTransform().setPosition(vec3(i, 0, j));
			}
		}
	}
	calculateWallMesh();
}

void Map::addActor(Actor* actor) {
	actors.push_back(actor);
}

void Map::draw() {	
	for (Actor* actor : actors) {
		actor->draw();
	}

	mapMaterial.use();
	const Shader* shader = mapMaterial.getShader();
	shader->use();
	mat4 M = glm::translate(glm::vec3(0.5,-0.5f,0.5));
	shader->setUniformValue("u_M", M);
	shader->setUniformValue("u_iTM", glm::mat3(glm::transpose(glm::inverse(M))));
	mapMesh.draw();
}

void Map::update(float deltaTime) {	
	for (Actor* actor : actors) {
		actor->update(deltaTime);
	}
	std::cerr << "Upd";
}

void Map::calculateWallMesh()
{
	using WallType = Wall::Type;

	std::vector<Vertex> vertices;
	for(int x = 0; x < mapSize; x++)
	{
		for(int z = 0; z < mapSize; z++)
		{
			auto it = walls.find(ivec2(x,z));
			if(it == walls.end())
			{
				vertices.push_back(Vertex { vec3(x, 0, z + 1), vec3(0, 1, 0), vec2(0, 0.5f)});
				vertices.push_back(Vertex { vec3(x + 1, 0, z), vec3(0, 1, 0), vec2(0.5f, 0)});
				vertices.push_back(Vertex { vec3(x, 0, z), vec3(0, 1, 0), vec2(0, 0)});
				
				vertices.push_back(Vertex { vec3(x, 0, z + 1), vec3(0, 1, 0), vec2(0, 0.5f)});
				vertices.push_back(Vertex { vec3(x + 1, 0, z + 1), vec3(0, 1, 0), vec2(0.5f, 0.5f)});
				vertices.push_back(Vertex { vec3(x + 1, 0, z), vec3(0, 1, 0), vec2(0.5f, 0)});
			}
			else
			{
				WallType type = it->second->getType();
				vec2 uv;
				switch(type)
				{
					default:
						uv = glm::vec2(0.0f, 0.0f);
						break;

					case WallType::Wood:
						uv = glm::vec2(0.5f, 0.0f);
						break;
						
					case WallType::Stone:
						uv = glm::vec2(0.0f, 0.5f);
						break;

					case WallType::Metal:
						uv = glm::vec2(0.5f, 0.5f);
						break;
				}

				vertices.push_back(Vertex { vec3(x, 1, z + 1), vec3(0, 1, 0), uv + glm::vec2(0.0f, 0.5f)});
				vertices.push_back(Vertex { vec3(x + 1, 1, z), vec3(0, 1, 0), uv + glm::vec2(0.5f, 0.0f)});
				vertices.push_back(Vertex { vec3(x, 1, z), vec3(0, 1, 0), uv + glm::vec2(0.0f, 0.0f)});
				
				vertices.push_back(Vertex { vec3(x, 1, z + 1), vec3(0, 1, 0), uv + glm::vec2(0.0f, 0.5f)});
				vertices.push_back(Vertex { vec3(x + 1, 1, z + 1), vec3(0, 1, 0), uv + glm::vec2(0.5f, 0.5f)});
				vertices.push_back(Vertex { vec3(x + 1, 1, z), vec3(0, 1, 0), uv + glm::vec2(0.5f, 0.0f)});
				
				if(auto it = walls.find(ivec2(x + 1,z)); it == walls.end())
				{
					vertices.push_back(Vertex { vec3(x + 1, 0, z), vec3(1, 0, 0), uv + glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex { vec3(x + 1, 1, z), vec3(1, 0, 0), uv + glm::vec2(0.5f, 0.0f)});
					vertices.push_back(Vertex { vec3(x + 1, 0, z + 1), vec3(1, 0, 0), uv + glm::vec2(0.0f, 0.5f)});
					
					vertices.push_back(Vertex { vec3(x + 1, 0, z + 1), vec3(1, 0, 0), uv + glm::vec2(0.0f, 0.5f)});
					vertices.push_back(Vertex { vec3(x + 1, 1, z), vec3(1, 0, 0), uv + glm::vec2(0.5f, 0.0f)});
					vertices.push_back(Vertex { vec3(x + 1, 1, z + 1), vec3(1, 0, 0), uv + glm::vec2(0.5f, 0.5f)});
				}

				if(auto it = walls.find(ivec2(x - 1,z)); it == walls.end())
				{
					vertices.push_back(Vertex { vec3(x, 0, z), vec3(-1, 0, 0), uv + glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex { vec3(x, 0, z + 1), vec3(-1, 0, 0), uv + glm::vec2(0.0f, 0.5f)});
					vertices.push_back(Vertex { vec3(x, 1, z), vec3(-1, 0, 0), uv + glm::vec2(0.5f, 0.0f)});
					
					vertices.push_back(Vertex { vec3(x, 0, z + 1), vec3(-1, 0, 0), uv + glm::vec2(0.0f, 0.5f)});
					vertices.push_back(Vertex { vec3(x, 1, z + 1), vec3(-1, 0, 0), uv + glm::vec2(0.5f, 0.5f)});
					vertices.push_back(Vertex { vec3(x, 1, z), vec3(-1, 0, 0), uv + glm::vec2(0.5f, 0.0f)});
				}

				if(auto it = walls.find(ivec2(x,z + 1)); it == walls.end())
				{
					vertices.push_back(Vertex { vec3(x, 0, z + 1), vec3(0, 0, 1), uv + glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex { vec3(x + 1, 0, z + 1), vec3(0, 0, 1), uv + glm::vec2(0.5f, 0.0f)});
					vertices.push_back(Vertex { vec3(x, 1, z + 1), vec3(0, 0, 1), uv + glm::vec2(0.0f, 0.5f)});
					
					vertices.push_back(Vertex { vec3(x + 1, 0, z + 1), vec3(0, 0, 1), uv + glm::vec2(0.5f, 0.0f)});
					vertices.push_back(Vertex { vec3(x + 1, 1, z + 1), vec3(0, 0, 1), uv + glm::vec2(0.5f, 0.5f)});
					vertices.push_back(Vertex { vec3(x, 1, z + 1), vec3(0, 0, 1), uv + glm::vec2(0.0f, 0.5f)});
				}

				if(auto it = walls.find(ivec2(x,z - 1)); it == walls.end())
				{
					vertices.push_back(Vertex { vec3(x, 0, z), vec3(0, 0, -1), uv + glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex { vec3(x, 1, z), vec3(0, 0, -1), uv + glm::vec2(0.0f, 0.5f)});
					vertices.push_back(Vertex { vec3(x + 1, 0, z), vec3(0, 0, -1), uv + glm::vec2(0.5f, 0.0f)});
					
					vertices.push_back(Vertex { vec3(x + 1, 0, z), vec3(0, 0, -1), uv + glm::vec2(0.5f, 0.0f)});
					vertices.push_back(Vertex { vec3(x, 1, z), vec3(0, 0, -1), uv + glm::vec2(0.0f, 0.5f)});
					vertices.push_back(Vertex { vec3(x + 1, 1, z), vec3(0, 0, -1), uv + glm::vec2(0.5f, 0.5f)});
				}
			}
		}
	}
	mapMesh = Mesh(vertices);
}
