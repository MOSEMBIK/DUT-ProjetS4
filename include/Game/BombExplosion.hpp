#pragma once

#include <Game/Actor.hpp>
#include <Engine/Event/EventReceiver.hpp>

class BombExplosion : public Actor, public IEventReceiver
{
    float m_max_time;

public:
    float m_time;
    BombExplosion(Map* map, glm::vec3 position, float time = 1.5f);
    virtual ~BombExplosion();

    virtual void onEvent(Event& e);
    bool onUpdate(AppUpdateEvent& e);
};