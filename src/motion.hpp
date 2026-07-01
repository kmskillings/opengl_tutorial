#ifndef _MOTION_PATH_HPP
#define _MOTION_PATH_HPP

#include <memory>

#include "gl_includes.h"

#include "scene.hpp"

namespace GlWorld
{

class Transform;

class MotionRotate : public SceneElement
{

public:

    MotionRotate(
        std::shared_ptr<Transform> transform,
        const float& rotationsPerSecond,
        const glm::vec3& axis
    );

    bool caresAboutUpdatePhysical(void) const;

    bool caresAboutUpdateVisual(void) const;

    bool caresAboutRenderPass(void) const;

    void updatePhysicalPre(
        const Scene& scene,
        const float& secondsDelta
    );

    void updateVisualPre(
        const Scene& scene,
        const float& secondsDelta
    );

    void updatePhysical(
        const Scene& scene,
        const float& secondsDelta
    );

    void updateVisual(
        const Scene& scene,
        const float& secondsDelta
    );

    void updatePhysicalPost(
        const Scene& scene,
        const float& secondsDelta
    );

    void updateVisualPost(
        const Scene& scene,
        const float& secondsDelta
    );

    void draw(
        const Scene& scene
    );

private:

    std::shared_ptr<Transform> transform;
    float rotationsPerSecond;
    glm::vec3 axis;

};

}

#endif