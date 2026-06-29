#include <memory>

#include "gl_includes.h"
#include "motionPath.hpp"

#include "transform.hpp"

namespace GlWorld
{

void positionTransform(void);

MotionPathCircular::MotionPathCircular(
    const float &radius,
    const glm::vec3 &center,
    const glm::vec3 &normal,
    const float &period,
    const glm::vec3 &starting
)
{
    this->center = center;
    this->normal = glm::normalize(normal);
    this->period = period;
    this->offset = glm::normalize(
        starting
        - glm::dot(this->normal, starting) * this->normal
    ) * radius;

    this->transform = std::make_shared<Transform>();
    this->transform->setPosition(
        this->center + this->offset
    );
}

void MotionPathCircular::tick(
    const float& secondsDelta
)
{
    glm::quat q = glm::quat(
        2 * M_PI * secondsDelta / this->period,
        this->normal
    );

    this->offset = q * this->offset;
    
    this->transform->setPosition(this->center + this->offset);
}

void MotionPathCircular::attachTransform(
    std::shared_ptr<Transform> transform
)
{
    this->transform = transform;
    this->transform->setPosition(this->center + this->offset);
}

std::shared_ptr<Transform> MotionPathCircular::getAttachedTransform(void) const
{
    return this->transform;
}

}