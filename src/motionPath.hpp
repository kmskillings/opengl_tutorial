#ifndef _MOTION_PATH_HPP
#define _MOTION_PATH_HPP

#include <memory>

#include "gl_includes.h"

namespace GlWorld
{

class Transform;

class MotionPath
{

public:

    virtual void tick(float secondsDelta) = 0;

    virtual std::shared_ptr<Transform> getAttachedTransform(void) const = 0;

    virtual void attachTransform(
        std::shared_ptr<Transform> transform
    ) = 0;

};

class MotionPathCircular : public MotionPath
{

public:

    MotionPathCircular(
        const float &radius,
        const glm::vec3 &center,
        const glm::vec3 &normal,
        const float &period,        // In revolutions per second
        const glm::vec3 &starting   // Vector pointing to where the path starts (at phase 0.0f)
    );

    void tick(
        const float& secondsDelta
    );

    void attachTransform(
        std::shared_ptr<Transform> transform
    );

    std::shared_ptr<Transform> getAttachedTransform(void) const;

private:
    glm::vec3 center;
    glm::vec3 normal;
    float period;
    glm::vec3 offset;
    std::shared_ptr<Transform> transform;
};

}

#endif