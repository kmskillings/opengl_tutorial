#ifndef _MOUSE_MODE_SWITCH_SYSTEM_HPP
#define _MOUSE_MODE_SWITCH_SYSTEM_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "inputEvent.hpp"

template<typename T>
struct FixedPackedArray;

class MouseModeSwitchSystem
{

private:

    int key_;
    bool active_;

public:

    MouseModeSwitchSystem(void);

    MouseModeSwitchSystem(
        int key
    );

    void setMouseMode(
        const FixedPackedArray<InputEvent>& inputEvents,
        bool& keyUpMode,
        bool& keyDownMode
    );

};

#endif