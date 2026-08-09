#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "playerMovementSystem.hpp"

#include "playerControlState.hpp"

PlayerMovementSystem::PlayerMovementSystem(void) :
    movementSpeed_(0.0f),
    lookSensitivity_(0.0f),
    rollSpeed_(0.0f)
{}

PlayerMovementSystem::PlayerMovementSystem(
    const float& movementSpeed,
    const float& lookSensitivity,
    const float& rollSpeed
) :
    movementSpeed_(movementSpeed),
    lookSensitivity_(lookSensitivity),
    rollSpeed_(rollSpeed)
{}

float PlayerMovementSystem::getMovementSpeed(void)
{
    return movementSpeed_;
}

void PlayerMovementSystem::setMovementSpeed(
    const float& movementSpeed
)
{
    movementSpeed_ = movementSpeed;
}

float PlayerMovementSystem::getLookSensitivity(void)
{
    return lookSensitivity_;
}

void PlayerMovementSystem::setLookSensitivity(
    const float& lookSensitivity
)
{
    lookSensitivity_ = lookSensitivity;
}

void PlayerMovementSystem::update(
    const float& secondsDelta,
    const PlayerControlState& playerControlState,
    bool cameraOrientationActive,
    glm::vec3& playerPosition,
    glm::quat& playerOrientation
)
{
    float mouseXDelta = static_cast<float>(playerControlState.mouseXDelta);
    float mouseYDelta = static_cast<float>(playerControlState.mouseYDelta);

    float yaw = mouseXDelta * lookSensitivity_;
    float pitch = mouseYDelta * lookSensitivity_;
    
    if (cameraOrientationActive)
    {
        glm::quat yawQuat = glm::angleAxis(yaw, glm::vec3(0.0f, -1.0f, 0.0f));
        glm::quat pitchQuat = glm::angleAxis(pitch, glm::vec3(-1.0f, 0.0f, 0.0f));
        float roll = playerControlState.roll * rollSpeed_ * secondsDelta;
        glm::quat rollQuat = glm::angleAxis(roll, glm::vec3(0.0f, 0.0f, 1.0f));
        playerOrientation = playerOrientation * yawQuat * pitchQuat * rollQuat;
    }

    glm::vec3 playerVelocityLocal;
    if (
        playerControlState.right == 0 &&
        playerControlState.up == 0 &&
        playerControlState.forward == 0
    )
    {
        playerVelocityLocal = glm::vec3(0.0f);
    }
    else
    {
        playerVelocityLocal = glm::normalize(glm::vec3(
            static_cast<float>(playerControlState.right),
            static_cast<float>(playerControlState.up),
            static_cast<float>(-playerControlState.forward)
        )) * movementSpeed_;
    }
    glm::vec3 playerVelocity = playerOrientation * playerVelocityLocal;
    playerPosition = playerPosition + playerVelocity * secondsDelta;

}