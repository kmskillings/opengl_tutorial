#include "modelTexturedSimple.hpp"

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
    const glm::mat4 &matrixModel,
    const glm::mat4 &matrixView,
    const glm::mat4 &matrixProject 
) const
{
    glm::mat4 matrix = matrixProject * matrixView * matrixModel;
    this->material.get()->setMatrix(matrix);
    this->material.get()->activate();
    this->mesh.get()->activate();
    this->mesh.get()->draw();
}

}