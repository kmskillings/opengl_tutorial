#ifndef _SCENE_HPP
#define _SCENE_HPP

#include "gl_includes.h"

#include <vector>

#include "gameObject.hpp"

namespace GlWorld {

class Scene {
    public:
        Scene(glm::vec4 skyColor);
        void draw(void);
    private:
        glm::vec4 skyColor;
        std::vector<GlWorld::GameObject> gameObjects;
        
};

}

#endif