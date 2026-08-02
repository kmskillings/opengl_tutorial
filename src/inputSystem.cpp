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

void handleMouseMotionEvent(
    GLFWwindow* window,
    double x,
    double y
);

void handleMouseButtonEvent(
    GLFWwindow* window,
    int button,
    int action,
    int mods
);

void handleScrollwheelEvent(
    GLFWwindow* window,
    double xOffset,
    double yOffset
);

void InputSystem::init(
    GLFWwindow* window
)
{
    window_ = window;
    glfwSetKeyCallback(window_, handleKeyEvent);
    glfwSetCursorPosCallback(window_, handleMouseMotionEvent);
    glfwSetMouseButtonCallback(window_, handleMouseButtonEvent);
    glfwSetScrollCallback(window_, handleScrollwheelEvent);
    mouseLastPopulated_ = false;
}

void InputSystem::getInputs(
    FixedPackedArray<InputEvent>& inputArray
)
{
    inputArray.clear();
    inputArrayPtr_ = &inputArray;
    glfwSetWindowUserPointer(
        window_, 
        static_cast<void*>(this)
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
    InputSystem* system = 
        static_cast<InputSystem*>(glfwGetWindowUserPointer(window));
    system->inputArrayPtr_->push(KeyboardEvent{
        key,
        action
    });
}

void handleMouseMotionEvent(
    GLFWwindow* window,
    double x,
    double y
)
{
    InputSystem* system = 
        static_cast<InputSystem*>(glfwGetWindowUserPointer(window));
    double mouseXLast;
    double mouseYLast;
    if (system->mouseLastPopulated_)
    {
        mouseXLast = system->mouseXLast_;
        mouseYLast = system->mouseYLast_;
    }
    else
    {
        mouseXLast = x;
        mouseYLast = y;
        system->mouseLastPopulated_ = true;
    }
    system->inputArrayPtr_->push(MouseMotionEvent{
        mouseXLast,
        mouseYLast,
        x,
        y
    });
    system->mouseXLast_ = x;
    system->mouseYLast_ = y;
}

void handleMouseButtonEvent(
    GLFWwindow* window,
    int button,
    int action,
    int mods
)
{
    InputSystem* system = 
        static_cast<InputSystem*>(glfwGetWindowUserPointer(window));
    system->inputArrayPtr_->push(MouseButtonEvent{
        button,
        action
    });
}

void handleScrollwheelEvent(
    GLFWwindow* window,
    double xOffset,
    double yOffset
)
{
    InputSystem* system = 
        static_cast<InputSystem*>(glfwGetWindowUserPointer(window));
    system->inputArrayPtr_->push(ScrollwheelEvent{
        yOffset
    });
}