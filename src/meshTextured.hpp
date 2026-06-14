#ifndef _MESH_TEXTURED_HPP
#define _MESH_TEXTURED_HPP

#include <vector>
#include <memory>

#include "gl_includes.h"

namespace GlWorld
{

class MeshTextured
{
public:
    MeshTextured(
        const std::vector<float> &vertices,
        const std::vector<GLuint> &elements
    );
    void bindPosition(GLuint location);
    void bindTextureCoords(GLuint location);
    void activate(void);
    void deactivate(void);
    void draw(void);
    static std::shared_ptr<MeshTextured> cube(float sideLength);
private:
    std::vector<float> vertices;
    std::vector<GLuint> elements;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint locationPosition;
    GLuint locationTextureCoords;
};

}

#endif