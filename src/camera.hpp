#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include <memory>

#include "transform.hpp"

namespace GlWorld
{

class Camera : public HasTransform
{
public:
    Camera(
        Transform* transform,
        float fovVert, 
        float aspect, 
        float near, 
        float far
    );
    Transform* getTransform(void) const;
    glm::mat4 getMatrixView(void) const;
    glm::mat4 getMatrixProject(void) const;
private:
    Transform* transform;
    float fovVert;
    float aspect;
    float near;
    float far;
};

}

#endif