#include <memory>

#include "gl_includes.h"
#include "motion.hpp"

#include "transform.hpp"

namespace GlWorld
{

MotionRotate::MotionRotate(
    Transform* transform,
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



MotionCamera::MotionCamera(
    GLFWwindow* window,
    const float& speedTranslate,
    const float& speedRotate,
    Transform* transformCamera
) :
    window(window),
    speedTranslate(speedTranslate),
    speedRotate(speedRotate),
    transformCamera(std::move(transformCamera))
{

}

bool MotionCamera::caresAboutUpdatePhysical(void) const
{
    return true;
}

void MotionCamera::updatePhysical(
    const Scene& scene,
    const float& secondsDelta
)
{
    glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
    if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
    {
        direction = direction + glm::vec3(-1.0f, 0.0f, 0.0f);
    }
    if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
    {
        direction = direction + glm::vec3(1.0f, 0.0f, 0.0f);
    }
    if (glfwGetKey(this->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        direction = direction + glm::vec3(0.0f, -1.0f, 0.0f);
    }
    if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        direction = direction + glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
    {
        direction = direction + glm::vec3(0.0f, 0.0f, -1.0f);
    }
    if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
    {
        direction = direction + glm::vec3(0.0f, 0.0f, 1.0f);
    }
    if (glm::length(direction) > 0.1f)
        direction = glm::normalize(direction);
    glm::vec3 velocity = direction * this->speedTranslate;
    this->transformCamera->translate(velocity * secondsDelta, TransformAxes::Local);

    double mousePositionNowX;
    double mousePositionNowY;
    glfwGetCursorPos(this->window, &mousePositionNowX, &mousePositionNowY);
    double mousePositionDeltaX = mousePositionNowX - this->mousePositionLastX;
    double mousePositionDeltaY = mousePositionNowY - this->mousePositionLastY;

    float pitchAngle = -mousePositionDeltaY * this->speedRotate * secondsDelta;
    this->transformCamera->rotate(
        pitchAngle, 
        glm::vec3(1.0f, 0.0f, 0.0f), 
        TransformAxes::Local
    );

    float yawAngle = -mousePositionDeltaX * this->speedRotate * secondsDelta;
    this->transformCamera->rotate(
        yawAngle,
        glm::vec3(0.0f, 1.0f, 0.0f),
        TransformAxes::Local
    );

    this->mousePositionLastX = mousePositionNowX;
    this->mousePositionLastY = mousePositionNowY;
}

MotionRevolve::MotionRevolve(
    const glm::vec3& center,
    const glm::vec3& axis,
    const float& revolutionsPerSecond,
    Transform* transform
) :
    center(std::make_unique<Transform>()),
    centerRotation(std::make_unique<MotionRotate>(
        this->center.get(), 
        revolutionsPerSecond, 
        axis
    )),
    transform(transform),
    elements(std::unordered_set<SceneElement*>())
{
    this->elements.insert(this->centerRotation.get());
    this->center->setPosition(center, TransformAxes::Global);
    this->transform->setParent(this->center.get(), TransformAxes::Global);
}

const std::unordered_set<SceneElement*>& MotionRevolve::getSceneElements(void) const
{
    return this->elements;
}

}