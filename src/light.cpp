#include "gl_includes.h"
#include "transform.hpp"
#include "light.hpp"

namespace GlWorld
{

LightDirectional::LightDirectional(
    Transform* transform,
    glm::vec3 color
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

void LightDirectional::setColor(glm::vec3 color)
{
    this->color = color;
}

glm::vec4 LightDirectional::getDirection(void) const
{
    glm::vec4 direction = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
    direction = this->getTransform()->getMatrixModel() * direction;
    direction = glm::normalize(direction);
    return direction;
}

LightAmbient::LightAmbient(glm::vec3 color)
{
    this->color = color;
}

glm::vec3 LightAmbient::getColor(void) const
{
    return this->color;
}

void LightAmbient::setColor(glm::vec3 color)
{
    this->color = color;
}

}