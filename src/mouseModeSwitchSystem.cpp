#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mouseModeSwitchSystem.hpp"

#include "inputEvent.hpp"
#include "fixedPackedArray.hpp"

MouseModeSwitchSystem::MouseModeSwitchSystem(void) :
    key_(0),
    active_(false)
{}

MouseModeSwitchSystem::MouseModeSwitchSystem(
    int key
) :
    key_(key),
    active_(true)
{}

void MouseModeSwitchSystem::setMouseMode(
    const FixedPackedArray<InputEvent>& inputEvents,
    bool& keyUpMode,
    bool& keyDownMode
)
{
    for (int i = 0; i < inputEvents.count; i++)
    {
        InputEvent inputEvent = inputEvents[i];
        if (!std::holds_alternative<KeyboardEvent>(inputEvent))
        {
            continue;
        }

        KeyboardEvent kbEvent = std::get<KeyboardEvent>(inputEvent);
        if (kbEvent.key == key_ && active_)
        {
            if (kbEvent.action == GLFW_PRESS)
            {
                keyUpMode = false;
                keyDownMode = true;
            }
            else if (kbEvent.action == GLFW_RELEASE)
            {
                keyUpMode = true;
                keyDownMode = false;
            }
        }
    }
}