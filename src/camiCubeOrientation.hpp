#ifndef _CAMI_CUBE_ORIENTATION
#define _CAMI_CUBE_ORIENTATION

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct CamiCubeOrientation
{
    float orientationAngle;
    glm::vec3 orientationAxis;
    float rotationRate;
    glm::vec3 rotationAxis;
};

#endif