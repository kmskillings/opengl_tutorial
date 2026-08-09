#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "sphereOrientationSystem.hpp"

#include "playerControlState.hpp"

SphereOrientationSystem::SphereOrientationSystem(void) :
    mouseSensitivity_(0.0f),
    rollSpeed_(0.0f)
{}

SphereOrientationSystem::SphereOrientationSystem(
    float mouseSensitivity,
    float rollSpeed
) :
    mouseSensitivity_(mouseSensitivity),
    rollSpeed_(rollSpeed)
{}

void SphereOrientationSystem::updateSphereOrientation(
    float secondsDelta,
    const PlayerControlState& controlState,
    bool sphereOrientationActive,
    const glm::quat& cameraOrientation,
    glm::quat& sphereOrientation
)
{
    if (!sphereOrientationActive)
    {
        return;
    }

    glm::vec3 pitchAxis = cameraOrientation * glm::vec3(1.0f, 0.0f, 0.0f);
    float pitch = controlState.mouseYDelta * mouseSensitivity_;
    glm::quat pitchQuat = glm::angleAxis(pitch, pitchAxis);

    glm::vec3 yawAxis = cameraOrientation * glm::vec3(0.0f, 1.0, 0.0f);
    float yaw = controlState.mouseXDelta * mouseSensitivity_;
    glm::quat yawQuat = glm::angleAxis(yaw, yawAxis);

    glm::vec3 rollAxis = cameraOrientation * glm::vec3(0.0f, 0.0f, 1.0f);
    float roll = controlState.roll * rollSpeed_ * secondsDelta;
    glm::quat rollQuat = glm::angleAxis(roll, rollAxis);

    sphereOrientation = yawQuat * pitchQuat * rollQuat * sphereOrientation;

}