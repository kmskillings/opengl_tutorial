#ifndef _COLLISION_NARROW_SYSTEM_HPP
#define _COLLISION_NARROW_SYSTEM_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

template<typename T>
struct FixedPackedArray;

struct Collision;

class CollisionNarrowSystem
{

private:

public:

void detectCollisions(
    float cubeSideLength,
    float sphereRadius,
    const glm::vec3& spherePosition,
    const FixedPackedArray<glm::mat4> transforms,
    const FixedPackedArray<Collision>& mediumPhaseCollisions,
    FixedPackedArray<Collision>& narrowPhaseCollisions
) const;

};

#endif