#ifndef _PLAYER_MOVEMENT_SYSTEM
#define _PLAYER_MOVEMENT_SYSTEM

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct PlayerControlState;

class PlayerMovementSystem
{

private:

    float movementSpeed_;
    float lookSensitivity_;
    float rollSpeed_;

public:

    PlayerMovementSystem(void);
    
    PlayerMovementSystem(
        const float& movementSpeed,
        const float& lookSensitivity,
        const float& rollSpeed
    );

    float getMovementSpeed(void);
    void setMovementSpeed(const float& movementSpeed);

    float getLookSensitivity(void);
    void setLookSensitivity(const float& lookSensitivity);

    void update(
        const float& secondsDelta,
        const PlayerControlState& playerControlState,
        bool cameraOrientationActive,
        glm::vec3& playerPosition,
        glm::quat& playerOrientation
    );

};

#endif