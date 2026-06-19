#ifndef _SCENE_HPP
#define _SCENE_HPP

#include "gl_includes.h"

#include <vector>
#include <memory>

namespace GlWorld {

class Camera;
class LightAmbient;
class LightDirectional;
class WorldObject;

class Scene {
public:
    Scene(
        std::shared_ptr<Camera> camera,
        glm::vec4 skyColor,
        std::shared_ptr<LightAmbient> lightAmbient,
        std::shared_ptr<LightDirectional> lightDirectional
    );
    std::shared_ptr<Camera> getCamera(void) const;
    std::shared_ptr<LightAmbient> getLightAmbient(void) const;
    std::shared_ptr<LightDirectional> getLightDirectional(void) const;
    void addWorldObject(std::shared_ptr<WorldObject> worldObject);
    void draw(void);
private:
    glm::vec4 skyColor;
    std::shared_ptr<Camera> camera;
    std::vector<std::shared_ptr<WorldObject>> worldObjects;
    std::shared_ptr<LightAmbient> lightAmbient;
    std::shared_ptr<LightDirectional> lightDirectional;
};

}

#endif