#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "playerControlSystem.hpp"

#include <variant>

#include "fixedPackedArray.hpp"
#include "inputEvent.hpp"
#include "playerControlState.hpp"
#include "doubleBuffer.hpp"

void setAxis(
    int8_t& axis,
    const InputEvent& inputEvent,
    const int& positiveKey,
    const int& negativeKey
);

void PlayerControlSystem::update(
    const FixedPackedArray<InputEvent>& inputEvents,
    PlayerControlState& playerControlState
)
{
    playerControlState.mouseXDelta = 0;
    playerControlState.mouseYDelta = 0;
    for (int i = 0; i < inputEvents.count; i++)
    {
        const InputEvent& inputEvent = inputEvents[i];
        setAxis(
            playerControlState.forward, 
            inputEvent, 
            GLFW_KEY_W, 
            GLFW_KEY_S
        );
        setAxis(
            playerControlState.right,
            inputEvent,
            GLFW_KEY_D,
            GLFW_KEY_A
        );
        setAxis(
            playerControlState.up,
            inputEvent,
            GLFW_KEY_SPACE,
            GLFW_KEY_LEFT_SHIFT
        );
        setAxis(
            playerControlState.roll,
            inputEvent,
            GLFW_KEY_Q,
            GLFW_KEY_E
        );

        if (std::holds_alternative<MouseMotionEvent>(inputEvent))
        {
            MouseMotionEvent mmEvent = std::get<MouseMotionEvent>(inputEvent);
            playerControlState.mouseXDelta = mmEvent.toX - mmEvent.fromX;
            playerControlState.mouseYDelta = mmEvent.toY - mmEvent.fromY;
        }
    }
    playerControlState.normalize();
}

void setAxis(
    int8_t& axis,
    const InputEvent& inputEvent,
    const int& positiveKey,
    const int& negativeKey
)
{
    if (!std::holds_alternative<KeyboardEvent>(inputEvent))
    {
        return;
    }

    KeyboardEvent kbEvent = std::get<KeyboardEvent>(inputEvent);

    if (kbEvent.key == positiveKey)
    {
        if (kbEvent.action == GLFW_PRESS)
        {
            axis = axis + 1;
        }
        else if (kbEvent.action == GLFW_RELEASE)
        {
            axis = axis - 1;
        }
    }
    else if (kbEvent.key == negativeKey)
    {
        if (kbEvent.action == GLFW_PRESS)
        {
            axis = axis - 1;
        }
        else if (kbEvent.action == GLFW_RELEASE)
        {
            axis = axis + 1;
        }
    }
}