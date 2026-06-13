#ifndef _MODEL_HPP
#define _MODEL_HPP

#include "gl_includes.h"
#include "material.hpp"
#include "mesh.hpp"

namespace GlWorld {

class Model {
public:
    Model(glm::vec4 color){
        // Bind any vertex attributes used by the Material
        this->material = new Material(color);
        this->mesh = Mesh::Triangle(1.0f, 1.0f);
        this->material->bindVertexAttributes(this->mesh);
    }
    void drawRgb(void) {
        this->material->activate();     // Call useShaderProgram on any shader programs connected to the material and send any material-stored uniforms.
        this->mesh->activate();         // Bind any vaos of the mesh
        this->mesh->draw();             // Make the appropriate draw calls on the mesh
        this->mesh->deactivate();       // Un-use any shader programs
        this->material->deactivate();   // Unbind any vaos of the mesh.
    }
    void setColor(glm::vec4 color) {
        this->material->setColor(color);
    }
private:
    Material* material;
    Mesh* mesh;
};

}

#endif