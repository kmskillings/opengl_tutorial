#ifndef _INPUT_EVENT_HPP
#define _INPUT_EVENT_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Input Event
// 
// An Input Event represents a single event relating to player input.

struct InputEvent
{

    enum class Kind
    {
        Key
    };

    Kind kind;
    int key;
    int action;

};

#endif