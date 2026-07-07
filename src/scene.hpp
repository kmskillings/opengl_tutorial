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

class Scene;
class SceneElement
{

public:

    // Get whether this SceneElement should be notified of physics updates
    // and visual updates and rendering passes, as appropriate

    virtual bool caresAboutUpdatePhysical(void) const;

    virtual bool caresAboutUpdateVisual(void) const;

    virtual bool caresAboutRenderPass(void) const;

    // Pre-update functions are called first, and should be used to inspect
    // other aspects of the Scene if required.

    virtual void updatePhysicalPre(
        const Scene& scene,
        const float& secondsDelta
    );

    virtual void updateVisualPre(
        const Scene& scene,
        const float& secondsDelta
    );

    // Update functions are called second, and should actually make the update.

    virtual void updatePhysical(
        const Scene& scene,
        const float& secondsDelta
    );

    virtual void updateVisual(
        const Scene& scene,
        const float& secondsDelta
    );

    // Post-update functions are called third, and should perform any required
    // clean-up.

    virtual void updatePhysicalPost(
        const Scene& scene,
        const float& secondsDelta
    );

    virtual void updateVisualPost(
        const Scene& scene,
        const float& secondsDelta
    );

    virtual void draw(
        const Scene& scene
    );

};

template<class T>
class SceneElementGroup : public SceneElement
{

public:

    virtual const T& getSceneElements(void) const = 0;

    bool caresAboutUpdatePhysical(void) const override
    {
        T elements = this->getSceneElements();
        for (SceneElement* element : elements)
        {
            if (element->caresAboutUpdatePhysical())
            {
                return true;
            }
        }
        return false;
    }

    bool caresAboutUpdateVisual(void) const override
    {
        T elements = this->getSceneElements();
        for (SceneElement* element : elements)
        {
            if (element->caresAboutUpdateVisual())
            {
                return true;
            }
        }
        return false;
    }

    bool caresAboutRenderPass(void) const override
    {
        T elements = this->getSceneElements();
        for (SceneElement* element : elements)
        {
            if (element->caresAboutRenderPass())
            {
                return true;
            }
        }
        return false;
    }

    void updatePhysicalPre(
        const Scene& scene,
        const float& secondsDelta
    ) override
    {
        T elements = this->getSceneElements();
        for (SceneElement* element : elements)
        {
            if (element->caresAboutUpdatePhysical())
            {
                element->updatePhysicalPre(scene, secondsDelta);
            }
        }
    }

    void updatePhysical(
        const Scene& scene,
        const float& secondsDelta
    ) override
    {
        T elements = this->getSceneElements();
        for (SceneElement* element : elements)
        {
            if (element->caresAboutUpdatePhysical())
            {
                element->updatePhysical(scene, secondsDelta);
            }
        }
    }

    void updatePhysicalPost(
        const Scene& scene,
        const float& secondsDelta
    ) override
    {
        T elements = this->getSceneElements();
        for (SceneElement* element : elements)
        {
            if (element->caresAboutUpdatePhysical())
            {
                element->updatePhysicalPost(scene, secondsDelta);
            }
        }
    }

    void updateVisualPre(
        const Scene& scene,
        const float& secondsDelta
    ) override
    {
        T elements = this->getSceneElements();
        for (SceneElement* element : elements)
        {
            if (element->caresAboutUpdateVisual())
            {
                element->updateVisualPre(scene, secondsDelta);
            }
        }
    }

    void updateVisual(
        const Scene& scene,
        const float& secondsDelta
    ) override
    {
        T elements = this->getSceneElements();
        for (SceneElement* element : elements)
        {
            if (element->caresAboutUpdateVisual())
            {
                element->updateVisual(scene, secondsDelta);
            }
        }
    }

    void updateVisualPost(
        const Scene& scene,
        const float& secondsDelta
    ) override
    {
        T elements = this->getSceneElements();
        for (SceneElement* element : elements)
        {
            if (element->caresAboutUpdateVisual())
            {
                element->updateVisualPost(scene, secondsDelta);
            }
        }
    }

    void draw(
        const Scene& scene
    ) override
    {
        T elements = this->getSceneElements();
        for (SceneElement* element : elements)
        {
            if (element->caresAboutRenderPass())
            {
                element->draw(scene);
            }
        }
    }

};

class Transform;
class Scene :
    public SceneElementGroup<std::unordered_set<SceneElement*>>
{

private:

    glm::vec4 skyColor;

    // The scene is an observer of the camera. An external entity (the "world?")
    // owns it. The same is true for each SceneElement.
    Camera* camera;
    std::unordered_set<SceneElement*> elements;
    LightAmbient* lightAmbient;
    LightDirectional* lightDirectional;

    std::unique_ptr<Transform> transformBase;

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

    void physicsTick(const float& secondsDelta);

    void visualTick(const float& secondsDelta);

    void render(void);

    const std::unordered_set<SceneElement*>& getSceneElements(void) const override;

    const Transform& getTransformBase(void) const;

};

}

#endif