#ifndef _SCENE_HPP
#define _SCENE_HPP

#include "gl_includes.h"

#include <vector>
#include <memory>

#include "camera.hpp"
#include "worldObject.hpp"

namespace GlWorld {

class Scene {
public:
    Scene(
        std::shared_ptr<Camera> camera,
        glm::vec4 skyColor
    );
    std::shared_ptr<Camera> getCamera(void) const;
    void addWorldObject(std::shared_ptr<WorldObject> worldObject);
    void draw(void);
private:
    glm::vec4 skyColor;
    std::shared_ptr<Camera> camera;
    std::vector<std::shared_ptr<WorldObject>> worldObjects;
};

}

#endif