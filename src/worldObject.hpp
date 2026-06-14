#ifndef _GAME_OBJECT_HPP
#define _GAME_OBJECT_HPP

#include "gl_includes.h"

#include "transform.hpp"
#include "hasTransform.hpp"

namespace GlWorld {

class WorldObject : HasTransform
{
public:
    std::shared_ptr<Transform> getTransform(void) const;
    void draw(
        const glm::mat4 &matrixView,
        const glm::mat4 &matrixProject
    ) const;
private:
    std::shared_ptr<Transform> transform;
    std::shared_ptr<Model> model;
};

}

#endif