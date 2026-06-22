#ifndef _MODEL_HPP
#define _MODEL_HPP

#include <memory>

#include "gl_includes.h"

namespace GlWorld {

class Scene;
class WorldObject;
class Mesh;
class Material;

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
        std::shared_ptr<Mesh> mesh,
        std::shared_ptr<Material> material
    );
    void draw(
        const Scene &scene,
        const WorldObject &worldObject
    ) const;
private:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
};

}

#endif