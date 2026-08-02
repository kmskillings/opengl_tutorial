#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camiCubeTransformSystem.hpp"

#include "fixedPackedArray.hpp"
#include "collision.hpp"
#include "camiCubeOrientation.hpp"

void CamiCubeTransformSystem::calculateTransforms(
    float secondsElapsed,
    const FixedPackedArray<Collision>& collisions,
    const FixedPackedArray<glm::vec3>& positions,
    const FixedPackedArray<CamiCubeOrientation>& orientations,
    FixedPackedArray<glm::mat4>& transforms
) const
{
    transforms.clear();

    int i;
    Collision collision;
    glm::vec3 position;
    CamiCubeOrientation orientation;
    for (
        i = 0;
        i < collisions.count &&
        collisions[i].camiCubeIndex < orientations.count &&
        collisions[i].camiCubeIndex < positions.count &&
        transforms.count <= transforms.capacity - 1;
        i++
    )
    {
        collision = collisions[i];
        position = positions[collision.camiCubeIndex];
        orientation = orientations[collision.camiCubeIndex];
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, position);
            transform = glm::rotate(
            transform,
            orientation.rotationRate * secondsElapsed,
            orientation.rotationAxis
        );
        transform = glm::rotate(
            transform, 
            orientation.orientationAngle, 
            orientation.orientationAxis
        );
        transforms.push(transform);
    }
}