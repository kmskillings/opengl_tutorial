#include "scene.hpp"

namespace GlWorld {

    Scene::Scene(glm::vec4 skyColor)
    {
        this->skyColor = skyColor;
    }

    void Scene::draw(void)
    {
        
        glClearColor(this->skyColor.r, this->skyColor.g, this->skyColor.b, this->skyColor.a);
        glClear(GL_COLOR_BUFFER_BIT);

        // Iterate through the GameObjects and draw them.

    }

}