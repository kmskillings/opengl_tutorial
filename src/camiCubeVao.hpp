#ifndef _CAMI_CUBE_VAO_HPP
#define _CAMI_CUBE_VAO_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "world.hpp"

// Cami Cube Vao
//
// A CamiCubeVao is responsible for managing the VAO of a set of instanced
// Cami Cubes. It provides methods for modifying aspects of the instances.
// It does not handle any of the actual rendering; that is the responsibility
// of a separate render system.

class CamiCubeVao
{

private:

    GLuint vao_;
    GLuint vboVertices_;
    GLuint vboInstances_;
    GLuint ebo_;

    uint triangleCount_;    // Per instance, not total
    uint instanceCount_;

public:

    struct CamiCubeInstance
    {
        float       scale;
        float       orientationAngle;
        glm::vec3   orientationAxis;
        float       rotationRate;
        glm::vec3   rotationAxis;
        glm::vec3   modelPosition;
        glm::vec3   color;
    };

    CamiCubeVao(
        uint instanceCount,
        glm::vec3* instancePositions,
        CamiCubeOrientation* instanceOrientations
    );

    ~CamiCubeVao(
        void
    );

    GLuint getVao(void) const;

    GLuint getEbo(void) const;

    uint getTriangleCount(void) const;

    uint getInstanceCount(void) const;

};

#endif