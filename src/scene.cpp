#include "gl_includes.h"

#include "scene.hpp"

#include <vector>
#include <memory>
#include <unordered_set>
#include <stdio.h>

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
    lightDirectional(lightDirectional),
    transformBase(std::make_unique<Transform>())
{
    glGenFramebuffers(1, &this->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Create color attachment as a texture
    glGenTextures(1, &this->textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, this->textureColorBuffer);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        800,
        600,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        NULL
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, 
        GL_COLOR_ATTACHMENT0, 
        GL_TEXTURE_2D, 
        textureColorBuffer,
        0
    );

    // Create depth/stencil attachment and renderbuffer
    glGenRenderbuffers(1, &this->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, 
        GL_DEPTH_STENCIL_ATTACHMENT, 
        GL_RENDERBUFFER, 
        this->rbo
    );

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Framebuffer is not complete.");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);

    glClearColor(
        this->skyColor.r,
        this->skyColor.g,
        this->skyColor.b,
        this->skyColor.a
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->draw(*this);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const Transform& Scene::getTransformBase(void) const
{
    return *this->transformBase.get();
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