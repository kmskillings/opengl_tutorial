#include "worldObject.hpp"

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
    const glm::mat4 &matrixView,
    const glm::mat4 &matrixProject
) const
{
    glm::mat4 matrixModel = this->getTransform().get()->getMatrixModel();
    this->model.get()->draw(matrixModel, matrixView, matrixProject);
}

}