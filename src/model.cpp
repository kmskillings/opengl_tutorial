#include "model.hpp"
#include "material.hpp"
#include "mesh.hpp"

namespace GlWorld {

Model::Model(
    Transform* transform,
    Material* material,
    Mesh* mesh
) : 
    mesh(std::move(mesh)),
    material(std::move(material)),
    transform(std::move(transform))
{

}

Transform* Model::getTransform(void) const
{
    return this->transform;
}

bool Model::caresAboutUpdatePhysical(void) const
{
    return false;
}

bool Model::caresAboutUpdateVisual(void) const
{
    return false;
}

bool Model::caresAboutRenderPass(void) const
{
    return true;
}

void Model::updatePhysicalPre(
    const Scene& scene,
    const float& secondsDelta
)
{
    return;
}

void Model::updateVisualPre(
    const Scene& scene,
    const float& secondsDelta
)
{
    return;
}

void Model::updatePhysical(
    const Scene& scene,
    const float& secondsDelta
)
{
    return;
}

void Model::updateVisual(
    const Scene& scene,
    const float& secondsDelta
)
{
    return;
}

void Model::updatePhysicalPost(
    const Scene& scene,
    const float& secondsDelta
)
{
    return;
}

void Model::updateVisualPost(
    const Scene& scene,
    const float& secondsDelta
)
{
    return;
}

void Model::draw(
    const Scene& scene

)
{
    this->material->activate(scene, *this->transform);
    this->material->bindMesh(*this->mesh); 
    this->mesh->activate();
    this->mesh->draw();
    this->mesh->deactivate();
    this->material->deactivate();
}

}