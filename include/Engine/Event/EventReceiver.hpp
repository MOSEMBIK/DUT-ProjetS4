#pragma once

#include <Engine/Event/Event.hpp>
#include <Engine/Event/MouseEvent.hpp>
#include <Engine/Event/KeyEvent.hpp>
#include <Engine/Event/ApplicationEvent.hpp>

class IEventReceiver {
public:
    virtual void onEvent(Event& event) = 0;
    virtual void onAttached() { };
    virtual void onDetached() { };
};