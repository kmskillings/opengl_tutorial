#include "worldObject.hpp"

#include "scene.hpp"
#include "model.hpp"

#include <memory>

namespace GlWorld
{

WorldObject::WorldObject(std::shared_ptr<Model> model)
{
    this->model = model;
    this->transform = std::make_shared<Transform>();
}

std::shared_ptr<Transform> WorldObject::getTransform(void) const
{
    return this->transform;
}

void WorldObject::draw(
    const Scene &scene
) const
{
    this->model->draw(scene, *this);
}

}