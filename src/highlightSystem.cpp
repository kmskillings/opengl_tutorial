#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "highlightSystem.hpp"

#include <string>

#include "fixedPackedArray.hpp"
#include "collision.hpp"
#include "instanceAttribute.hpp"

HighlightSystem::HighlightSystem(
    uint32_t instanceSize,
    uint32_t colorOffset,
    const glm::vec3& highlightColor
) :
    instanceSize_(instanceSize),
    colorOffset_(colorOffset),
    highlightColor_(highlightColor)
{}

void HighlightSystem::getColorUpdates(
    const FixedPackedArray<Collision>& appeared,
    const FixedPackedArray<Collision>& disappeared,
    FixedPackedArray<InstanceAttribute>& updateAttributes,
    FixedPackedArray<uint32_t>& instanceIndexes,
    FixedPackedArray<unsigned char>& updateData
) const
{
    int i;
    for (
        i = 0;
        i < appeared.count &&
        updateAttributes.count <= updateAttributes.capacity - 1 &&
        updateData.count <= updateData.capacity - sizeof(glm::vec3) &&
        instanceIndexes.count <= instanceIndexes.capacity - 1;
        i++
    )
    {
        updateAttributes.push(InstanceAttribute(
            instanceSize_,
            sizeof(glm::vec3),
            colorOffset_
        ));
        instanceIndexes.push(appeared[i].camiCubeIndex);
        std::memcpy(
            &updateData[updateData.count], 
            &highlightColor_, 
            sizeof(glm::vec3)
        );
        updateData.count += sizeof(glm::vec3);
    }

    for (
        i = 0;
        i < disappeared.count &&
        updateAttributes.count <= updateAttributes.capacity - 1 &&
        updateData.count <= updateData.capacity - sizeof(glm::vec3) &&
        instanceIndexes.count <= instanceIndexes.capacity - 1;
        i++
    )
    {
        updateAttributes.push(InstanceAttribute(
            instanceSize_,
            sizeof(glm::vec3),
            colorOffset_
        ));
        instanceIndexes.push(disappeared[i].camiCubeIndex);
        glm::vec3 white(1.0f);
        std::memcpy(
            &updateData[updateData.count], 
            &white, 
            sizeof(glm::vec3)
        );
        updateData.count += sizeof(glm::vec3);
    }
}