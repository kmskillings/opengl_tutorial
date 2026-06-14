#ifndef _MODEL_SOLID_COLOR
#define _MODEL_SOLID_COLOR

#include "model.hpp"
#include "meshTextured.hpp"

namespace GlWorld 
{

class ModelTexturedSimple : Model 
{
public:
    ModelTexturedSimple(
        std::shared_ptr<MeshTextured> mesh,
        std::shared_ptr<MaterialTexturedSimple> material
    );
    void draw(
        const glm::mat4 &matrixModel,
        const glm::mat4 &matrixView,
        const glm::mat4 &matrixProject 
    );
private:
    std::shared_ptr<MeshTextured> mesh;
    std::shared_ptr<MaterialTexturedSimple> material;
};

}

#endif