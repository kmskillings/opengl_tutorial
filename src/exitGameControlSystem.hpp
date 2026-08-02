#ifndef _EXIT_GAME_CONTROL_SYSTEM_HPP
#define _EXIT_GAME_CONTROL_SYSTEM_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "inputEvent.hpp"

template<typename T>
struct FixedPackedArray;

class ExitGameControlSystem
{

private:

GLFWwindow* window_;

public:

ExitGameControlSystem(
    GLFWwindow* window_
);

void updateWindowShouldClose(
    const FixedPackedArray<InputEvent>& inputEvents
) const;

};

#endif