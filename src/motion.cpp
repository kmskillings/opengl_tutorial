#include <memory>

#include "gl_includes.h"
#include "motion.hpp"

#include "transform.hpp"

namespace GlWorld
{

MotionRotate::MotionRotate(
    std::shared_ptr<Transform> transform,
    const float& rotationsPerSecond,
    const glm::vec3& axis
) :
    transform(std::move(transform)),
    rotationsPerSecond(rotationsPerSecond),
    axis(axis)
{

}

bool MotionRotate::caresAboutUpdatePhysical(void) const
{
    return true;
}

bool MotionRotate::caresAboutUpdateVisual(void) const
{
    return false;
}

bool MotionRotate::caresAboutRenderPass(void) const
{
    return false;
}

void MotionRotate::updatePhysicalPre(
    const Scene& scene,
    const float& secondsDelta
)
{
    return;
}

void MotionRotate::updatePhysical(
    const Scene& scene,
    const float& secondsDelta
)
{
    this->transform->rotate(
        2.0f * M_PI * this->rotationsPerSecond * secondsDelta,
        this->axis
    );
}

void MotionRotate::updatePhysicalPost(
    const Scene& scene,
    const float& secondsDelta
)
{
    return;
}

void MotionRotate::updateVisualPre(
    const Scene& scene,
    const float& secondsDelta
)
{
    return;
}

void MotionRotate::updateVisual(
    const Scene& scene,
    const float& secondsDelta
)
{
    return;
}

void MotionRotate::updateVisualPost(
    const Scene& scene,
    const float& secondsDelta
)
{
    return;
}

void MotionRotate::draw(
    const Scene& scene
)
{
    return;
}

}