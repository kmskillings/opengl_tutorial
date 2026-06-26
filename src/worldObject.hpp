#ifndef _GAME_OBJECT_HPP
#define _GAME_OBJECT_HPP

#include "gl_includes.h"
#include "transform.hpp"

#include <memory>

namespace GlWorld {

class Model;
class Scene;
class Transform;

class WorldObject : public HasTransform
{
public:
    WorldObject(
        std::shared_ptr<Model> model,
        std::shared_ptr<Transform> transform
    );
    std::shared_ptr<Transform> getTransform(void) const;
    void draw(
        const Scene &scene
    ) const;
private:
    std::shared_ptr<Transform> transform;
    std::shared_ptr<Model> model;
};

}

#endif