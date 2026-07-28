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
    lookSensitivity_(0.0f)
{}

PlayerMovementSystem::PlayerMovementSystem(
    const float& movementSpeed,
    const float& lookSensitivity
) :
    movementSpeed_(movementSpeed),
    lookSensitivity_(lookSensitivity)
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
    glm::vec3& playerPosition,
    glm::quat& playerOrientation
)
{
    glm::vec3 playerVelocity;
    if (
        playerControlState.right == 0 &&
        playerControlState.up == 0 &&
        playerControlState.forward == 0
    )
    {
        playerVelocity = glm::vec3(0.0f);
    }
    else
    {
        playerVelocity = glm::normalize(glm::vec3(
            static_cast<float>(playerControlState.right),
            static_cast<float>(playerControlState.up),
            static_cast<float>(-playerControlState.forward)
        )) * movementSpeed_;
    }
    playerPosition = playerPosition + playerVelocity * secondsDelta;
}