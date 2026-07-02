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

bool MotionCamera::caresAboutUpdateVisual(void) const
{
    return false;
}

bool MotionCamera::caresAboutRenderPass(void) const
{
    return false;
}

void MotionCamera::updatePhysicalPre(
    const Scene& scene,
    const float& secondsDelta
)
{

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
    velocity = glm::mat3(this->transformCamera->getMatrixModel()) * velocity;
    this->transformCamera->translate(velocity * secondsDelta);

    double mousePositionNowX;
    double mousePositionNowY;
    glfwGetCursorPos(this->window, &mousePositionNowX, &mousePositionNowY);
    double mousePositionDeltaX = mousePositionNowX - this->mousePositionLastX;
    double mousePositionDeltaY = mousePositionNowY - this->mousePositionLastY;

    glm::vec3 pitchAxis = glm::mat3(this->transformCamera->getMatrixModel()) * glm::vec3(1.0f, 0.0f, 0.0f);
    pitchAxis = glm::normalize(pitchAxis);
    float pitchAngle = -mousePositionDeltaY * this->speedRotate * secondsDelta;
    this->transformCamera->rotate(pitchAngle, pitchAxis);

    glm::vec3 yawAxis = glm::mat3(this->transformCamera->getMatrixModel()) * glm::vec3(0.0f, 1.0f, 0.0f);
    yawAxis = glm::normalize(yawAxis);
    float yawAngle = -mousePositionDeltaX * this->speedRotate * secondsDelta;
    this->transformCamera->rotate(yawAngle, yawAxis);

    this->mousePositionLastX = mousePositionNowX;
    this->mousePositionLastY = mousePositionNowY;
}

void MotionCamera::updatePhysicalPost(
    const Scene& scene,
    const float& secondsDelta
)
{

}

void MotionCamera::updateVisualPre(
    const Scene& scene,
    const float& secondsDelta
)
{

}

void MotionCamera::updateVisual(
    const Scene& scene,
    const float& secondsDelta
)
{

}

void MotionCamera::updateVisualPost(
    const Scene& scene,
    const float& secondsDelta
)
{

}

void MotionCamera::draw(const Scene& scene)
{

}

}