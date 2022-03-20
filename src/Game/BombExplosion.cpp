#include <Game/BombExplosion.hpp>
#include <Game/Game.hpp>

using namespace glm;

BombExplosion::BombExplosion(Map* map, vec3 position, float time) : Actor(map), m_max_time(time), m_time(time)
{
    m_transform.setPosition(position);

    std::vector<vec3> positions = { vec3(-0.5, -0.5, -0.5), vec3(0.5, -0.5, 0.5), vec3(-0.5, 0.5, -0.5), vec3(0.5, 0.5, 0.5), vec3(-0.5, -0.5, 0.5), vec3(0.5, -0.5, -0.5), vec3(-0.5, 0.5, 0.5), vec3(0.5, 0.5, -0.5) };
    std::vector<vec3> normals = { vec3(0.5, 0, -0.5), vec3(-0.5, 0, 0.5), vec3(0.5, 0, -0.5), vec3(-0.5, 0, 0.5), vec3(0.5, 0, 0.5), vec3(-0.5, 0, -0.5), vec3(0.5, 0, 0.5), vec3(-0.5, 0, -0.5) };
    std::vector<vec2> uvs = { vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1), vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1) };
    std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 1, 4, 5, 6, 6, 7, 5 };

    m_meshes.push_back(new Mesh(Mesh::createFromVectors(positions, positions, uvs), indices, GL_TRIANGLES));

    Material mat = Material();
    mat.setDiffuseTexture(9);
    mat.SetAmbientColor(glm::vec3(1.0f));
    mat.setDiffuseColor(glm::vec3(1.0f));
    m_materials.push_back(mat);

    Game::getInstance()->getMainWindow()->attachEventReceiver(*this);
}

BombExplosion::~BombExplosion()
{
    Game::getInstance()->getMainWindow()->detachEventReceiver(*this);
    for(auto mesh : m_meshes)
    {
        delete mesh;
    }
}

void BombExplosion::onEvent(Event& e)
{
    EventDispatcher dispatcher(e);
	dispatcher.Dispatch<AppUpdateEvent>(std::bind(&BombExplosion::onUpdate, this, std::placeholders::_1));
}

bool BombExplosion::onUpdate(AppUpdateEvent& e)
{
    m_time -= e.GetDeltaTime();

    if(m_time <= m_max_time / 2.0f);
        m_transform.setScale(vec3(pow(m_time / (m_max_time / 2.0f), 4)));

    return false;
}