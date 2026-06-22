#include "model.hpp"
#include "material.hpp"
#include "mesh.hpp"

namespace GlWorld {

ModelTexturedSimple::ModelTexturedSimple(
    std::shared_ptr<Mesh> mesh,
    std::shared_ptr<Material> material
)
{
    this->mesh = mesh;
    this->material = material;

    
}

void ModelTexturedSimple::draw(
    const Scene &scene,
    const WorldObject &worldObject
) const
{
    this->material->activate(scene, worldObject);
    this->material->bindMesh(*this->mesh.get());
    this->mesh->activate();
    this->mesh->draw();
    this->material->deactivate();
    this->mesh->deactivate();
}

}