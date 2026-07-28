#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "playerControlSystem.hpp"

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
    playerControlState.mouseXLast = playerControlState.mouseXNow;
    playerControlState.mouseYLast = playerControlState.mouseYNow;
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

        if (inputEvent.kind == InputEvent::Kind::Mouse)
        {
            if (playerControlState.cursorInitialized == false)
            {
                playerControlState.cursorInitialized = true;
                playerControlState.mouseXLast = inputEvent.mouseX;
                playerControlState.mouseYLast = inputEvent.mouseY;
            }
            playerControlState.mouseXNow = inputEvent.mouseX;
            playerControlState.mouseYNow = inputEvent.mouseY;
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
    if (inputEvent.kind != InputEvent::Kind::Key)
    {
        return;
    }

    if (inputEvent.key == positiveKey)
    {
        if (inputEvent.action == GLFW_PRESS)
        {
            axis = axis + 1;
        }
        else if (inputEvent.action == GLFW_RELEASE)
        {
            axis = axis - 1;
        }
    }
    else if (inputEvent.key == negativeKey)
    {
        if (inputEvent.action == GLFW_PRESS)
        {
            axis = axis - 1;
        }
        else if (inputEvent.action == GLFW_RELEASE)
        {
            axis = axis + 1;
        }
    }
}