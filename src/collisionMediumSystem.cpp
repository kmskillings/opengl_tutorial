#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "collisionMediumSystem.hpp"

#include "fixedPackedArray.hpp"
#include "collision.hpp"

void CollisionMediumSystem::detectCollisions(
    float cubeRadius,
    float sphereRadius,
    const glm::vec3& spherePosition,
    const FixedPackedArray<glm::vec3> camiCubePositions,
    const FixedPackedArray<Collision>& broadPhaseCollisions,
    FixedPackedArray<Collision>& mediumPhaseCollisions
) const
{
    mediumPhaseCollisions.clear();

    for (
        int i = 0;
        i < broadPhaseCollisions.count && 
        mediumPhaseCollisions.count <= mediumPhaseCollisions.capacity - 1;
        i++
    )
    {
        const Collision& bpCollision = broadPhaseCollisions[i];
        const glm::vec3& camiCubePosition 
            = camiCubePositions[bpCollision.camiCubeIndex];
        glm::vec3 offset = camiCubePosition - spherePosition;
        float totalRadius = cubeRadius + sphereRadius;
        if (glm::dot(offset, offset) < (totalRadius * totalRadius))
        {
            mediumPhaseCollisions.push(bpCollision);
        }
    }
}