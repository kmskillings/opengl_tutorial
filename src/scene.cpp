#include "gl_includes.h"

#include "scene.hpp"

#include <vector>
#include <memory>
#include <unordered_set>

#include "camera.hpp"
#include "light.hpp"

namespace GlWorld {

Scene::Scene(
    std::shared_ptr<Camera> camera,
    glm::vec4 skyColor,
    std::shared_ptr<LightAmbient> lightAmbient,
    std::shared_ptr<LightDirectional> lightDirectional
)
{
    this->camera = camera;
    this->skyColor = skyColor;
    this->lightAmbient = lightAmbient;
    this->lightDirectional = lightDirectional;
}

std::shared_ptr<Camera> Scene::getCamera(void) const
{
    return this->camera;
}

std::shared_ptr<LightAmbient> Scene::getLightAmbient(void) const
{
    return this->lightAmbient;
}

std::shared_ptr<LightDirectional> Scene::getLightDirectional(void) const
{
    return this->lightDirectional;
}

void Scene::addElement(
    std::shared_ptr<SceneElement> element
)
{
    this->elements.insert(element);
}

void Scene::updatePhysical(
    const float& secondsDelta
)
{
    for (std::shared_ptr<SceneElement> element : this->elements)
    {
        if (element->caresAboutUpdatePhysical())
        {
            element->updatePhysicalPre(*this, secondsDelta);
        }
    }

    for (std::shared_ptr<SceneElement> element : this->elements)
    {
        if (element->caresAboutUpdatePhysical())
        {
            element->updatePhysical(*this, secondsDelta);
        }
    }

    for (std::shared_ptr<SceneElement> element : this->elements)
    {
        if (element->caresAboutUpdatePhysical())
        {
            element->updatePhysicalPost(*this, secondsDelta);
        }
    }
}

void Scene::updateVisual(
    const float& secondsDelta
)
{
    for (std::shared_ptr<SceneElement> element : this->elements)
    {
        if (element->caresAboutUpdateVisual())
        {
            element->updateVisualPre(*this, secondsDelta);
        }
    }

    for (std::shared_ptr<SceneElement> element : this->elements)
    {
        if (element->caresAboutUpdateVisual())
        {
            element->updateVisual(*this, secondsDelta);
        }
    }

    for (std::shared_ptr<SceneElement> element : this->elements)
    {
        if (element->caresAboutUpdateVisual())
        {
            element->updateVisualPost(*this, secondsDelta);
        }
    }
}

void Scene::draw(void)
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(this->skyColor.r, this->skyColor.g, this->skyColor.b, this->skyColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (std::shared_ptr<SceneElement> element : this->elements)
    {
        element->draw(*this);
    }
}

}