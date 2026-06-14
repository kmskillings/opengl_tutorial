#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include <memory>

#include "hasTransform.hpp"

namespace GlWorld
{

class Camera : HasTransform
{
public:
    Camera(
        std::shared_ptr<Transform> transform,
        float fovVert, 
        float aspect, 
        float near, 
        float far
    );
    std::shared_ptr<Transform> getTransform(void) const;
    glm::mat4 getMatrixView(void) const;
    glm::mat4 getMatrixProject(void) const;
private:
    std::shared_ptr<Transform> transform;
    float fovVert;
    float aspect;
    float near;
    float far;
};

}

#endif