#ifndef _SCENE_HPP
#define _SCENE_HPP

#include "gl_includes.h"

#include <vector>
#include <memory>
#include <unordered_set>

namespace GlWorld {

class Camera;
class LightAmbient;
class LightDirectional;

class SceneElement;

class Scene {

public:

    Scene(
        Camera* camera,
        glm::vec4 skyColor,
        LightAmbient* lightAmbient,
        LightDirectional* lightDirectional
    );

    Camera* getCamera(void) const;

    LightAmbient* getLightAmbient(void) const;

    LightDirectional* getLightDirectional(void) const;

    void addElement(SceneElement* sceneElement);

    void updatePhysical(const float& secondsDelta);

    void updateVisual(const float& secondsDelta);

    void draw(void);

private:

    glm::vec4 skyColor;

    // The scene is an observer of the camera. An external entity (the "world?")
    // owns it. The same is true for each SceneElement.
    Camera* camera;
    std::unordered_set<SceneElement*> elements;
    LightAmbient* lightAmbient;
    LightDirectional* lightDirectional;

};

class SceneElement
{

public:

    // Get whether this SceneElement should be notified of physics updates
    // and visual updates and rendering passes, as appropriate

    virtual bool caresAboutUpdatePhysical(void) const = 0;

    virtual bool caresAboutUpdateVisual(void) const = 0;

    virtual bool caresAboutRenderPass(void) const = 0;

    // Pre-update functions are called first, and should be used to inspect
    // other aspects of the Scene if required.

    virtual void updatePhysicalPre(
        const Scene& scene,
        const float& secondsDelta
    ) = 0;

    virtual void updateVisualPre(
        const Scene& scene,
        const float& secondsDelta
    ) = 0;

    // Update functions are called second, and should actually make the update.

    virtual void updatePhysical(
        const Scene& scene,
        const float& secondsDelta
    ) = 0;

    virtual void updateVisual(
        const Scene& scene,
        const float& secondsDelta
    ) = 0;

    // Post-update functions are called third, and should perform any required
    // clean-up.

    virtual void updatePhysicalPost(
        const Scene& scene,
        const float& secondsDelta
    ) = 0;

    virtual void updateVisualPost(
        const Scene& scene,
        const float& secondsDelta
    ) = 0;

    virtual void draw(
        const Scene& scene
    ) = 0;

};

}

#endif