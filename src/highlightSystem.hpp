#ifndef _HIGHLIGHT_SYSTEM_HPP
#define _HIGHLIGHT_SYSTEM_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "instanceAttribute.hpp"

template<typename T>
struct FixedPackedArray;

struct Collision;

class HighlightSystem
{

private:

    uint32_t instanceSize_;
    uint32_t colorOffset_;
    glm::vec3 highlightColor_;

public:

    HighlightSystem(
        uint32_t instanceSize,
        uint32_t colorOffset,
        const glm::vec3& highlightColor
    );

    void getColorUpdates(
        const FixedPackedArray<Collision>& appeared,
        const FixedPackedArray<Collision>& disappeared,
        FixedPackedArray<InstanceAttribute>& updateAttributes,
        FixedPackedArray<uint32_t>& instanceIndexes,
        FixedPackedArray<unsigned char>& updateData
    ) const;

};

#endif