#include "model.hpp"
#include "material.hpp"
#include "meshTextured.hpp"

namespace GlWorld {

ModelTexturedSimple::ModelTexturedSimple(
    std::shared_ptr<MeshTextured> mesh,
    std::shared_ptr<MaterialTexturedSimple> material
)
{
    this->mesh = mesh;
    this->material = material;

    GLuint locationPosition = this->material.get()->getLocationPosition();
    this->mesh.get()->bindPosition(locationPosition);
    GLuint locationTextureCoords = this->material.get()->getLocationTextureCoords();
    this->mesh.get()->bindTextureCoords(locationTextureCoords);
}

void ModelTexturedSimple::draw(
    const Scene &scene,
    const WorldObject &worldObject
) const
{
    this->material.get()->activate(scene, worldObject);
    this->mesh.get()->activate();
    this->mesh.get()->draw();
}

}