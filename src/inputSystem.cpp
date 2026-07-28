#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "inputSystem.hpp"

#include "inputEvent.hpp"
#include "fixedPackedArray.hpp"

void handleKeyEvent(
    GLFWwindow* window, 
    int key, 
    int scancode, 
    int action, 
    int mods
);

void handleMouseEvent(
    GLFWwindow* window,
    double x,
    double y
);

void InputSystem::init(
    GLFWwindow* window
)
{
    window_ = window;
    glfwSetKeyCallback(window_, handleKeyEvent);
    glfwSetCursorPosCallback(window_, handleMouseEvent);
}

void InputSystem::getInputs(
    FixedPackedArray<InputEvent>& inputArray
)
{
    inputArray.clear();
    glfwSetWindowUserPointer(
        window_, 
        static_cast<void*>(&inputArray)
    );
    glfwPollEvents();
}

void handleKeyEvent(
    GLFWwindow* window, 
    int key, 
    int scancode, 
    int action, 
    int mods
)
{
    FixedPackedArray<InputEvent>& inputArray = 
        *static_cast<FixedPackedArray<InputEvent>*>(glfwGetWindowUserPointer(window));
    inputArray.push({
        InputEvent::Kind::Key,
        key,
        action
    });
}

void handleMouseEvent(
    GLFWwindow* window,
    double x,
    double y
)
{
    FixedPackedArray<InputEvent>& inputArray = 
        *static_cast<FixedPackedArray<InputEvent>*>(glfwGetWindowUserPointer(window));
    inputArray.push({
        InputEvent::Kind::Mouse,
        0,
        0,
        x,
        y
    });
}