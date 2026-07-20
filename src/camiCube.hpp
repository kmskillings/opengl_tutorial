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
        glm::vec3   color;
    };

    CamiCube* camiCubes_;
    uint count_;
    uint capacity_;

    float secondsElapsed_;
    glm::mat4 matrixProjView_;

    GLuint vao_;
    GLuint vboVertices_;
    GLuint vboInstances_;
    GLuint ebo_;

    GLuint shader_;
    GLuint texture_;

    bool instancesDirty_;
    bool matrixDirty_;

public:

    typedef uint Id;

    struct Collision
    {
        Id id;
    };

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
        const glm::vec3&    position,
        const glm::vec3&    color
    );

    // Get an array of collisions with the given point. The collisions are
    // placed in the given array. Only up to the given maximum collisions will
    // be detected; no guarantees are made about which ones these will be if
    // more collisions actually exist. Returns the actual number of collisions
    // detected.
    uint getCollisionsPoint(
        const glm::vec3& point,
        Collision* collisionArray,
        const uint& maxCollisionCount
    );

    void setMatrix(
        const glm::mat4& matrixProjView
    );

    bool setColor(
        const Id& id, 
        const glm::vec3& color
    );

    // Updates all CamiCubes in the system.
    void update(
        const float& secondsDelta
    );

    void draw(void);

};

#endif