#ifndef _CAMI_CUBE_HPP
#define _CAMI_CUBE_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Cami Cubes are simple cubes textured with Cami's face. They float in 3D
// space and rotate around a fixed axis at a fixed rate.
// Because the cubes rotate at a fixed rate, a global time counter can be used
// to track the rotation of all cubes. Then all Cami Cube data is read-only and
// used only to create the model matrix.

class CamiCubeSystem
{

private:

    struct CamiCube
    {
        float       scale;
        float       orientationAngle;
        glm::vec3   orientationAxis;
        float       rotationRate;
        glm::vec3   rotationAxis;
        glm::vec3   modelPosition;
    };

    CamiCube* camiCubes_;
    uint count_;
    uint capacity_;

    float secondsElapsed_;
    glm::mat4 matrixProjView_;
    glm::vec3 spherePosition_;
    float sphereRadius_;

    GLuint vao_;
    GLuint vboVertices_;
    GLuint vboInstances_;
    GLuint ebo_;

    GLuint shader_;
    GLuint texture_;

    bool instancesDirty_;
    bool matrixDirty_;
    bool sphereDirty_;

public:

    CamiCubeSystem(uint capacity);

    ~CamiCubeSystem(void);
    

    // Insert a new CamiCube with the given parameters. Returns whether it was
    // successfull inserted.
    bool insert(
        const float&        scale,
        const float&        orientationAngle,
        const glm::vec3&    orientationAxis,
        const float&        rotationRate,
        const glm::vec3&    rotationAxis,
        const glm::vec3&    position
    );

    void setMatrixProjView(
        const glm::mat4& matrixProjView
    );

    void setCutoutSphere(
        const glm::vec3& position,
        const float& radius
    );

    // Updates all CamiCubes in the system.
    void update(
        const float& secondsDelta
    );

    void draw(void);

};

#endif