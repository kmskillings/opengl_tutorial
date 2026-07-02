#include "gl_includes.h"

#include "scene.hpp"

#include <vector>
#include <memory>
#include <unordered_set>

#include "camera.hpp"
#include "light.hpp"

namespace GlWorld {

Scene::Scene(
    Camera* camera,
    glm::vec4 skyColor,
    LightAmbient* lightAmbient,
    LightDirectional* lightDirectional
) :
    camera(camera),
    skyColor(skyColor),
    lightAmbient(lightAmbient),
    lightDirectional(lightDirectional)
{
    
}

Camera* Scene::getCamera(void) const
{
    return this->camera;
}

LightAmbient* Scene::getLightAmbient(void) const
{
    return this->lightAmbient;
}

LightDirectional* Scene::getLightDirectional(void) const
{
    return this->lightDirectional;
}

void Scene::addElement(
    SceneElement* element
)
{
    this->elements.insert(element);
}

void Scene::updatePhysical(
    const float& secondsDelta
)
{
    for (SceneElement* element : this->elements)
    {
        if (element->caresAboutUpdatePhysical())
        {
            element->updatePhysicalPre(*this, secondsDelta);
        }
    }

    for (SceneElement* element : this->elements)
    {
        if (element->caresAboutUpdatePhysical())
        {
            element->updatePhysical(*this, secondsDelta);
        }
    }

    for (SceneElement* element : this->elements)
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
    for (SceneElement* element : this->elements)
    {
        if (element->caresAboutUpdateVisual())
        {
            element->updateVisualPre(*this, secondsDelta);
        }
    }

    for (SceneElement* element : this->elements)
    {
        if (element->caresAboutUpdateVisual())
        {
            element->updateVisual(*this, secondsDelta);
        }
    }

    for (SceneElement* element : this->elements)
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
    for (SceneElement* element : this->elements)
    {
        element->draw(*this);
    }
}

}