#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "transform.hpp"

Transform::Transform(
    const glm::vec3& position,
    const glm::quat& orientation,
    const glm::vec3& scale
) :
    position_(position),
    orientation_(orientation),
    scale_(scale)
{

}

Transform::Transform(
    const glm::vec3& position,
    const float& orientationAngle,
    const glm::vec3& orientationAxis,
    const glm::vec3& scale
) :
    position_(position),
    orientation_(glm::angleAxis(
        orientationAngle,
        orientationAxis
    )),
    scale_(scale)
{

}

glm::mat4 Transform::getMatrix(void) const
{
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, position_);
    matrix = matrix * glm::mat4_cast(orientation_);
    matrix = glm::scale(matrix, scale_);
    return matrix;
}

glm::mat4 Transform::getMatrixInv(void) const
{
    return glm::inverse(getMatrix());
}

glm::vec3 Transform::getPosition(void) const
{
    return position_;
}

void Transform::setPosition(const glm::vec3& position)
{
    position_ = position;
}

void Transform::translate(
    const glm::vec3& offset, 
    const Axes& axes
)
{
    glm::vec3 offsetFixed = offset;
    if (axes == Axes::Local)
    {
        offsetFixed = orientation_ * offsetFixed;
    }
    position_ = position_ + offsetFixed;
}

void Transform::translate(
    const float& amount, 
    const glm::vec3& direction, 
    const Axes& axes
)
{
    glm::vec3 offset = direction * amount;
    translate(offset, axes);
}

glm::quat Transform::getOrientation(void) const
{
    return orientation_;
}

void Transform::setOrientation(const glm::quat& orientation)
{
    orientation_ = orientation;
}

void Transform::setOrientation(const float& angle, const glm::vec3& axis)
{
    setOrientation(glm::angleAxis(angle, axis));
}

void Transform::rotate(
    const glm::quat& rotation,
    const Axes& axes
)
{
    if (axes == Axes::Fixed)
    {
        orientation_ = rotation * orientation_;
    }
    else
    {
        orientation_ = orientation_ * rotation;
    }
}

void Transform::rotate(
    const float& angle,
    const glm::vec3& axis,
    const Axes& axes
)
{
    rotate(glm::angleAxis(angle, axis), axes);
}