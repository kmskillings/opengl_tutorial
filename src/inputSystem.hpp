#ifndef _INPUT_SYSTEM_HPP
#define _INPUT_SYSTEM_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "inputEvent.hpp"

template<typename T> struct FixedPackedArray;

// Input System
//
// The Input System is responsible for capturing the inputs and player
// commands. 

class InputSystem
{

private:

    GLFWwindow* window_;
    FixedPackedArray<InputEvent>* inputArrayPtr_;
    double mouseXLast_;
    double mouseYLast_;
    bool mouseLastPopulated_;

public:

    void init(
        GLFWwindow* window
    );

    void getInputs(
        FixedPackedArray<InputEvent>& inputArray
    );

    friend void handleKeyEvent(
        GLFWwindow* window, 
        int key, 
        int scancode, 
        int action, 
        int mods
    );

    friend void handleMouseMotionEvent(
        GLFWwindow* window,
        double x,
        double y
    );

    friend void handleMouseButtonEvent(
        GLFWwindow* window,
        int button,
        int action,
        int mods
    );

    friend void handleScrollwheelEvent(
        GLFWwindow* window,
        double xOffset,
        double yOffset
    );

};

#endif