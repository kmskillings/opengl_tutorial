#include "camera.hpp"

namespace GlWorld
{

Camera::Camera(
    std::shared_ptr<Transform> transform,
    float fovVert, 
    float aspect, 
    float near, 
    float far
)
{
    this->transform = transform;
    this->fovVert = fovVert;
    this->aspect = aspect;
    this->near = near;
    this->far = far;
}

std::shared_ptr<Transform> Camera::getTransform(void) const
{
    return this->transform;
}

glm::mat4 Camera::getMatrixView(void) const
{
    return this->getTransform().get()->getMatrixModelInv();
}

glm::mat4 Camera::getMatrixProject(void) const
{
    return glm::perspective(
        this->fovVert,
        this->aspect,
        this->near,
        this->far
    );
}

}