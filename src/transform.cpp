#include "gl_includes.h"

#include "transform.hpp"

namespace GlWorld {

Transform::Transform(void)
{
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotation = glm::zero<glm::quat>();
    this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

const glm::mat4 Transform::getMatrixModel(void)
{
    glm::mat4 identity = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(identity, glm::vec3(this->position));
    glm::mat4 rotate = glm::mat4_cast(this->rotation);
    glm::mat4 scale = glm::scale(identity, this->scale);
    return translate * rotate * scale;
}

const glm::mat4 Transform::getMatrixModelInv(void)
{
    return glm::inverse(this->getMatrixModel());
}

void Transform::setPosition(const glm::vec3 &position)
{
    this->position = glm::vec3(position);
}

void Transform::translate(const glm::vec3 &offset)
{
    this->position = this->position + offset;
}

void Transform::translate(const glm::vec3 &direction, float distance)
{
    glm::vec3 directionNormalized = glm::normalize(direction);
    this->position = this->position + direction * distance;
}

const glm::quat Transform::getRotation(void)
{
    return this->rotation;
}

void Transform::setRotation(const glm::quat &rotation)
{
    this->rotation = glm::quat(rotation);
}

void Transform::setRotation(float angle, const glm::vec3 axis)
{
    glm::quat q = glm::angleAxis(angle, axis);
    this->setRotation(q);
}

void Transform::rotate(const glm::quat &rotation)
{
    this->rotation = this->rotation + rotation;
}

void Transform::rotate(float angle, const glm::vec3 axis)
{
    glm::quat q = glm::angleAxis(angle, axis);
    this->rotate(q);
}

const glm::vec3 Transform::getScale(void)
{
    return this->scale;
}

void Transform::setScale(const glm::vec3 &scale)
{
    this->scale = glm::vec3(scale);
}

void Transform::scaleBy(const glm::vec3 &scale)
{
    this->scale = this->scale * scale;
}

}