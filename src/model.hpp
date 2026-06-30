#ifndef _MODEL_HPP
#define _MODEL_HPP

#include <memory>

#include "gl_includes.h"

#include "scene.hpp"
#include "transform.hpp"

namespace GlWorld {

class WorldObject;
class Mesh;
class Material;

class Model : 
    public SceneElement,
    public HasTransform
{

public:

    Model(
        std::shared_ptr<Transform> transform,
        std::shared_ptr<Material> material,
        std::shared_ptr<Mesh> mesh
    );

    bool caresAboutUpdatePhysical(void) const;

    bool caresAboutUpdateVisual(void) const;

    bool caresAboutRenderPass(void) const;

    void updatePhysicalPre(
        const Scene& scene,
        const float& secondsDelta
    );

    void updateVisualPre(
        const Scene& scene,
        const float& secondsDelta
    );

    void updatePhysical(
        const Scene& scene,
        const float& secondsDelta
    );

    void updateVisual(
        const Scene& scene,
        const float& secondsDelta
    );

    void updatePhysicalPost(
        const Scene& scene,
        const float& secondsDelta
    );

    void updateVisualPost(
        const Scene& scene,
        const float& secondsDelta
    );

    void draw(
        const Scene& scene
    );

private:

    std::shared_ptr<Transform> transform;
    std::shared_ptr<Material> material;
    std::shared_ptr<Mesh> mesh;

};

}

#endif