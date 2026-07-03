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

const std::unordered_set<SceneElement*>& Scene::getSceneElements(void) const
{
    return this->elements;
}

void Scene::addElement(
    SceneElement* element
)
{
    this->elements.insert(element);
}

void Scene::physicsTick(
    const float& secondsDelta
)
{
    this->updatePhysicalPre(*this, secondsDelta);
    this->updatePhysical(*this, secondsDelta);
    this->updatePhysicalPost(*this, secondsDelta);
}

void Scene::visualTick(
    const float& secondsDelta
)
{
    this->updateVisualPre(*this, secondsDelta);
    this->updateVisual(*this, secondsDelta);
    this->updateVisualPost(*this, secondsDelta);
}

void Scene::render(void)
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(
        this->skyColor.r,
        this->skyColor.g,
        this->skyColor.b,
        this->skyColor.a
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->draw(*this);
}

bool SceneElement::caresAboutUpdatePhysical(void) const
{
    return false;
}

bool SceneElement::caresAboutUpdateVisual(void) const
{
    return false;
}

bool SceneElement::caresAboutRenderPass(void) const
{
    return false;
}

void SceneElement::updatePhysicalPre(
    const Scene& scene,
    const float& secondsDelta
)
{

}

void SceneElement::updatePhysical(
    const Scene& scene,
    const float& secondsDelta
)
{

}

void SceneElement::updatePhysicalPost(
    const Scene& scene,
    const float& secondsDelta
)
{

}

void SceneElement::updateVisualPre(
    const Scene& scene,
    const float& secondsDelta
)
{

}

void SceneElement::updateVisual(
    const Scene& scene,
    const float& secondsDelta
)
{

}

void SceneElement::updateVisualPost(
    const Scene& scene,
    const float& secondsDelta
)
{

}

void SceneElement::draw(
    const Scene& scene
)
{

}

}