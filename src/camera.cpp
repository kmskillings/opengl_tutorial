#include "camera.hpp"

namespace GlWorld
{

Camera::Camera(
    Transform* transform,
    const float& fovVert, 
    const float& aspect, 
    const float& near, 
    const float& far
) :
    transform(transform),
    matrixProject(glm::perspective(
        fovVert,
        aspect,
        near,
        far
    ))
{
    
}

Transform* Camera::getTransform(void) const
{
    return this->transform;
}

glm::mat4 Camera::getMatrixView(void) const
{
    return this->getTransform()->getMatrixModelInv();
}

glm::mat4 Camera::getMatrixProject(void) const
{
    return this->matrixProject;
}

}