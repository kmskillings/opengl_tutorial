#ifndef _SPHERE_ORIENTATION_SYSTEM
#define _SPHERE_ORIENTATION_SYSTEM

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct PlayerControlState;

class SphereOrientationSystem
{

private:

    float mouseSensitivity_;
    float rollSpeed_;

public:

    SphereOrientationSystem(void);

    SphereOrientationSystem(
        float mouseSensitivity,
        float rollSpeed
    );

    void updateSphereOrientation(
        float secondsDelta,
        const PlayerControlState& controlState,
        bool sphereOrientationActive,
        const glm::quat& cameraOrientation,
        glm::quat& sphereOrientation
    );

};

#endif