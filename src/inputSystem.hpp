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

public:

    void init(
        GLFWwindow* window
    );

    void getInputs(
        FixedPackedArray<InputEvent>& inputArray
    );

};

#endif