#ifndef _LIGHT_HPP
#define _LIGHT_HPP

#include <memory>

#include "gl_includes.h"

#include "transform.hpp"

namespace GlWorld
{

class Transform;

class LightDirectional : public HasTransform
{
public:

    LightDirectional(
        std::shared_ptr<Transform> transform, 
        glm::vec3 color
    );

    std::shared_ptr<Transform> getTransform(void) const;

    glm::vec3 getColor(void) const;
    void setColor(glm::vec3 color);

    glm::vec4 getDirection(void) const;
    
private:

    std::shared_ptr<Transform> transform;

    glm::vec3 color;

};

class LightAmbient
{
public:

    LightAmbient(
        glm::vec3 color
    );

    glm::vec3 getColor(void) const;
    void setColor(glm::vec3 color);

private:

    glm::vec3 color;

};

}

#endif