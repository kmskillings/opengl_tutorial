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
        Transform* transform,
        const float& rotationsPerSecond,
        const glm::vec3& axis
    );

    bool caresAboutUpdatePhysical(void) const;

    void updatePhysical(
        const Scene& scene,
        const float& secondsDelta
    );

private:

    Transform* transform;
    float rotationsPerSecond;
    glm::vec3 axis;

};

class MotionCamera : public SceneElement
{

public:

    MotionCamera(
        GLFWwindow* window,
        const float& speedTranslate,
        const float& speedRotate,
        Transform* transformCamera
    );

    bool caresAboutUpdatePhysical(void) const;

    void updatePhysical(
        const Scene& scene,
        const float& secondsDelta
    );

private:

    GLFWwindow* window;

    Transform* transformCamera;

    float speedTranslate;
    float speedRotate;

    double mousePositionLastX;
    double mousePositionLastY;

};

}

#endif