#ifndef _INPUT_EVENT_HPP
#define _INPUT_EVENT_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <variant>

// Input Event
// 
// An Input Event represents a single event relating to player input.

struct KeyboardEvent
{
    int key;
    int action;
};

struct MouseMotionEvent
{
    double fromX;
    double fromY;
    double toX;
    double toY;
};

struct MouseButtonEvent
{
    int button;
    int action;
};

struct ScrollwheelEvent
{
    double scrollAmount;
};

typedef std::variant<
        KeyboardEvent,
        MouseMotionEvent,
        MouseButtonEvent,
        ScrollwheelEvent
    > InputEvent;

#endif