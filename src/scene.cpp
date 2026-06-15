#include "scene.hpp"

namespace GlWorld {

Scene::Scene(
    std::shared_ptr<Camera> camera,
    glm::vec4 skyColor
)
{
    this->camera = camera;
    this->skyColor = skyColor;
}

std::shared_ptr<Camera> Scene::getCamera(void) const
{
    return this->camera;
}

void Scene::addWorldObject(std::shared_ptr<WorldObject> worldObject)
{
    this->worldObjects.push_back(worldObject);
}

void Scene::draw(void)
{
    glm::mat4 matrixView = this->camera.get()->getMatrixView();
    glm::mat4 matrixProject = this->camera.get()->getMatrixProject();

    glEnable(GL_DEPTH_TEST);
    glClearColor(this->skyColor.r, this->skyColor.g, this->skyColor.b, this->skyColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (std::shared_ptr<WorldObject> worldObject : this->worldObjects)
    {
        worldObject.get()->draw(matrixView, matrixProject);
    }
}

}