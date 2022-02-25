#include <Engine/UI/Widget.hpp>

using namespace glm;
using namespace std;

Widget::Widget(Window* window, vec2 position, vec2 anchor = vec2(1.0f)) : m_window(window), m_position(position), m_anchor(anchor)
{

}
