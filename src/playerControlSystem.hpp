#ifndef _PLAYER_CONTROL_SYSTEM_HPP
#define _PLAYER_CONTROL_SYSTEM_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

template<typename T>
struct FixedPackedArray;
struct InputEvent;
struct PlayerControlState;

class PlayerControlSystem
{

private:

public:

    void update(
        const FixedPackedArray<InputEvent>& inputEvents,
        PlayerControlState& playerControlState
    );

};

#endif