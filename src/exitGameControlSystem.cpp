#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "exitGameControlSystem.hpp"

#include "fixedPackedArray.hpp"
#include "inputEvent.hpp"

ExitGameControlSystem::ExitGameControlSystem(
    GLFWwindow* window
) :
    window_(window)
{}

void ExitGameControlSystem::updateWindowShouldClose(
    const FixedPackedArray<InputEvent>& inputEvents
) const
{
    for (int i = 0; i < inputEvents.count; i++)
    {
        const InputEvent& inputEvent = inputEvents[i];
        if (!std::holds_alternative<KeyboardEvent>(inputEvent))
        {
            continue;
        }
        KeyboardEvent kbEvent = std::get<KeyboardEvent>(inputEvent);
        if (kbEvent.key == GLFW_KEY_ESCAPE && kbEvent.action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window_, GLFW_TRUE);
            return;
        }
    }
}