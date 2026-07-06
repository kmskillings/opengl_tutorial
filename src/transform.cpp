#include "gl_includes.h"

#include "transform.hpp"

#include <unordered_set>

namespace GlWorld {

Transform::Transform(void) :
    position(glm::vec3(0.0f)),
    rotation(glm::angleAxis(0.0f, glm::vec3(0.0f))),
    scale(glm::vec3(1.0f)),
    parent(&Transform::_zero)
{
    
}

glm::mat4 Transform::getMatrixModel(void) const
{
    return this->getMatrixModel(TransformAxes::Global);
}

glm::mat4 Transform::getMatrixModel(TransformAxes axes) const
{
    if (this == &Transform::_zero)
    {
        return glm::identity<glm::mat4>();
    }
    
    glm::mat4 matrixParent; // The matrix that transforms from the parent space
                            // to the target space.
    switch (axes)
    {
    case TransformAxes::Local:
        // The matrix required to transform from the model's local space to
        // the model's local space is obviously the identity matrix.
        return glm::identity<glm::mat4>();

    case TransformAxes::Parent:
        matrixParent = glm::identity<glm::mat4>();
        break;

    case TransformAxes::Global:
        matrixParent = this->parent->getMatrixModel(TransformAxes::Global);
        break;
    }

    glm::mat4 identity = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(identity, glm::vec3(this->position));
    glm::mat4 rotate = glm::mat4_cast(this->rotation);
    glm::mat4 scale = glm::scale(identity, this->scale);
    return matrixParent * translate * rotate * scale;
}

glm::mat4 Transform::getMatrixModelInv(void) const
{
    return this->getMatrixModelInv(TransformAxes::Global);
}

glm::mat4 Transform::getMatrixModelInv(TransformAxes axes) const
{
    return glm::inverse(this->getMatrixModel(axes));
}

glm::vec3 Transform::getPosition(void) const
{
    return this->getPosition(TransformAxes::Global);
}

glm::vec3 Transform::getPosition(TransformAxes axes) const
{
    switch (axes)
    {
    case TransformAxes::Local:
        return glm::vec3(0.0f);
        
    case TransformAxes::Parent:
        return this->position;

    case TransformAxes::Global:
        glm::vec4 position4 = glm::vec4(this->position, 1.0f);
        position4 = this->parent->getMatrixModel(TransformAxes::Global) * position4;
        return glm::vec3(position4);
    }
    return glm::vec3(0.0f);
}

void Transform::setPosition(const glm::vec3 &position)
{
    this->setPosition(position, TransformAxes::Global);
}

void Transform::setPosition(const glm::vec3 &position, TransformAxes axes)
{
    switch (axes)
    {
    case TransformAxes::Local:
        this->translate(position, TransformAxes::Local);
        return;
    case TransformAxes::Parent:
        this->position = position;
        return;
    case TransformAxes::Global:
        glm::vec4 position4 = glm::vec4(position, 1.0f);
        position4 = this->parent->getMatrixModelInv(TransformAxes::Global) * position4;
        this->position = glm::vec3(position4);
        return;
    }
}

void Transform::translate(const glm::vec3 &offset)
{
    this->translate(offset, TransformAxes::Global);
}

void Transform::translate(const glm::vec3 &offset, TransformAxes axes)
{
    glm::vec3 offsetParent;
    switch (axes)
    {
    case TransformAxes::Local:
        offsetParent = glm::mat3(this->getMatrixModel(TransformAxes::Parent)) * offset;
        break;
    case TransformAxes::Parent:
        offsetParent = offset;
        break;
    case TransformAxes::Global:
        offsetParent = glm::mat3(this->parent->getMatrixModelInv(TransformAxes::Global)) * offset;
        break;
    }
    this->position = this->position + offsetParent;
}

void Transform::translate(const glm::vec3 &direction, float distance)
{
    this->translate(direction, distance, TransformAxes::Global);
}

void Transform::translate(const glm::vec3& direction, float distance, TransformAxes axes)
{
    glm::vec3 directionNormalized = glm::normalize(direction);
    this->translate(directionNormalized * distance, axes);
}

glm::quat Transform::getRotation(void) const
{
    return this->getRotation(TransformAxes::Global);
}

glm::quat Transform::getRotation(TransformAxes axes) const
{
    if (this == &Transform::_zero)
    {
        return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    }

    switch (axes)
    {
    case TransformAxes::Local:
        return glm::quat(rotation);
    case TransformAxes::Parent:
        return this->rotation;
    case TransformAxes::Global:
        return 
            this->parent->getRotation(TransformAxes::Global)  *
            this->getRotation(TransformAxes::Parent)
        ;
    
    default:
        return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    }
}

void Transform::setRotation(const glm::quat &rotation)
{
    this->setRotation(rotation, TransformAxes::Global);
}

void Transform::setRotation(const glm::quat &rotation, TransformAxes axes)
{
    glm::quat newRotation;
    switch (axes)
    {
    case TransformAxes::Local:
        newRotation = this->rotation * rotation;
        break;
    case TransformAxes::Parent:
        newRotation = rotation;
        break;
    case TransformAxes::Global: {
        glm::quat rotationParentInv = glm::inverse(this->parent->getRotation(TransformAxes::Global));
        newRotation = rotationParentInv * rotation;
        break;
    }
    default:
        newRotation = rotation;
        break;
    }
    this->rotation = newRotation;
}

void Transform::setRotation(float angle, const glm::vec3 axis)
{
    this->setRotation(angle, axis, TransformAxes::Global);
}

void Transform::setRotation(float angle, const glm::vec3 axis, TransformAxes axes)
{
    glm::quat q = glm::angleAxis(angle, axis);
    this->setRotation(q, axes);
}

void Transform::rotate(const glm::quat &rotation)
{
    this->rotate(rotation, TransformAxes::Global);
}

void Transform::rotate(const glm::quat &rotation, TransformAxes axes)
{
    switch (axes)
    {
        case TransformAxes::Local:
        {
            this->setRotation(rotation, TransformAxes::Local);
            return;
        }
        case TransformAxes::Parent:
        {
            this->setRotation(
                rotation * this->getRotation(TransformAxes::Parent),
                TransformAxes::Parent
            );
            return;
        }
        case TransformAxes::Global:
        {
            this->setRotation(
                rotation * this->getRotation(TransformAxes::Global),
                TransformAxes::Global
            );
            return;
        }
        default:
        {
            return;
        }
    }
}

void Transform::rotate(float angle, const glm::vec3 axis)
{
    this->rotate(angle, axis, TransformAxes::Global);
}

void Transform::rotate(float angle, const glm::vec3 axis, TransformAxes axes)
{
    glm::quat q = glm::angleAxis(angle, axis);
    this->rotate(q, axes);
}

glm::vec3 Transform::getScale(void) const
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

Transform* Transform::getParent(void) const
{
    return this->parent;
}

void Transform::setParent(Transform* parent)
{
    this->setParent(parent, TransformAxes::Global);
}

void Transform::setParent(Transform* parent, TransformAxes axes)
{
    // The parent can only be set if the resulting parent graph will have
    // no cycles. This is detected by checking whether parent is equal to
    // this, meaning the transform will be its own parent, or whether this
    // is anywhere in the parent's parent set.
    std::unordered_set<Transform*> grandparents = parent->getParents();
    if (parent == this || grandparents.find(this) != grandparents.end())
    {
        return;
    }

    glm::vec3 preservePosition = this->getPosition(axes);
    glm::quat preserveRotation = this->getRotation(axes);

    this->parent = parent;

    this->setPosition(preservePosition, axes);
    this->setRotation(preserveRotation, axes);

}

std::unordered_set<Transform*> Transform::getParents(void)
{
    std::unordered_set<Transform*> parents = std::unordered_set<Transform*>();
    parents.insert(this->parent);
    bool parentsAdded = true;
    while (parentsAdded)
    {
        parentsAdded = false;
        for (Transform* t : parents)
        {
            parentsAdded = parentsAdded || (parents.insert(t->getParent())).second;
        }
    }
    return parents;
}

const Transform& Transform::zero(void)
{
    return Transform::_zero;
}

Transform Transform::_zero = Transform();

}