#include "gl_includes.h"
#include "transform.hpp"
#include "light.hpp"

namespace GlWorld
{

LightDirectional::LightDirectional(
    Transform* transform,
    const glm::vec3& color
)
{
    this->transform = transform;
    this->color = color;
}

Transform* LightDirectional::getTransform(void) const
{
    return this->transform;
}

glm::vec3 LightDirectional::getColor(void) const
{
    return this->color;
}

void LightDirectional::setColor(
    const glm::vec3& color
)
{
    this->color = color;
}

glm::vec4 LightDirectional::getDirection(void) const
{
    return glm::vec4(glm::normalize(this->transform->getPosition()), 0.0f);
}

LightAmbient::LightAmbient(
    const glm::vec3& color
)
{
    this->color = color;
}

glm::vec3 LightAmbient::getColor(void) const
{
    return this->color;
}

void LightAmbient::setColor(
    const glm::vec3& color
)
{
    this->color = color;
}

}