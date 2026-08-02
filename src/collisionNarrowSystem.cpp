#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "collisionNarrowSystem.hpp"

#include <algorithm>

#include "fixedPackedArray.hpp"
#include "collision.hpp"

void CollisionNarrowSystem::detectCollisions(
    float cubeSideLength,
    float sphereRadius,
    const glm::vec3& spherePosition,
    const FixedPackedArray<glm::mat4> transforms,
    const FixedPackedArray<Collision>& mediumPhaseCollisions,
    FixedPackedArray<Collision>& narrowPhaseCollisions
) const
{
    narrowPhaseCollisions.clear();

    int i;
    glm::mat4 transform;
    Collision mediumPhaseCollision;
    for (
        i = 0;
        i < transforms.count && 
        i < mediumPhaseCollisions.count &&
        narrowPhaseCollisions.count <= narrowPhaseCollisions.capacity - 1;
        i++
    )
    {
        transform = transforms[i];
        mediumPhaseCollision = mediumPhaseCollisions[i];
        glm::vec3 r = glm::vec3(
            glm::inverse(transform) * glm::vec4(spherePosition, 1.0f)
        );

        r = glm::abs(r);
        r = glm::max(
            glm::vec3(0.0f), 
            r - cubeSideLength * 0.5f
        );
        if (glm::dot(r, r) - (sphereRadius * sphereRadius) < 0.0f)
        {
            narrowPhaseCollisions.push(mediumPhaseCollision);
        }
    }
}