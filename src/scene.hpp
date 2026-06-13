#ifndef _SCENE_HPP
#define _SCENE_HPP

#include <vector>
#include "gameObject.hpp"

namespace GlWorld {

class Scene {
    public:
        void draw(void);
    private:
        std::vector<GlWorld::GameObject> gameObjects;
};

}

#endif