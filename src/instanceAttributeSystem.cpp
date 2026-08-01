#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "instanceAttributeSystem.hpp"

#include "fixedPackedArray.hpp"
#include "instanceAttribute.hpp"

void InstanceAttributeSystem::updateAttributes(
    unsigned int vao,
    unsigned int vbo,
    const FixedPackedArray<InstanceAttribute>& attributes,
    const FixedPackedArray<unsigned int>& instanceIndexes,
    const FixedPackedArray<unsigned char>& data
) const
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    for(
        int attributeCursor = 0, dataCursor = 0;
        attributeCursor < attributes.count &&
        attributeCursor < instanceIndexes.count &&
        dataCursor < data.count;
        attributeCursor++, dataCursor += attributes[attributeCursor].attributeSize
    )
    {
        const InstanceAttribute& attribute = attributes[attributeCursor];
        uint32_t instanceIndex = instanceIndexes[attributeCursor];
        glBufferSubData(
            GL_ARRAY_BUFFER,
            instanceIndex * attribute.instanceSize + attribute.attributeOffset,
            attribute.attributeSize,
            &data[dataCursor]
        );
    }
}