#ifndef _MODEL_HPP
#define _MODEL_HPP

#include <memory>

#include "gl_includes.h"

namespace GlWorld {

class Scene;
class WorldObject;
class MeshTextured;
class MaterialTexturedSimple;

class Model
{
public:
    virtual void draw(
        const Scene &scene,
        const WorldObject &worldObject
    ) const = 0;
private:
};

class ModelTexturedSimple : public Model 
{
public:
    ModelTexturedSimple(
        std::shared_ptr<MeshTextured> mesh,
        std::shared_ptr<MaterialTexturedSimple> material
    );
    void draw(
        const Scene &scene,
        const WorldObject &worldObject
    ) const;
private:
    std::shared_ptr<MeshTextured> mesh;
    std::shared_ptr<MaterialTexturedSimple> material;
};

}

#endif