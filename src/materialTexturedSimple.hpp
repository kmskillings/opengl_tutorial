#ifndef _MATERIAL_TEXTURED_SIMPLE_HPP
#define _MATERIAL_TEXTURED_SIMPLE_HPP

#include <memory>

#include "gl_includes.h"

namespace GlWorld
{

class MaterialTexturedSimple
{
public:
    MaterialTexturedSimple(GLuint shaderProgram);
    void setTexture(GLuint texture);
    void setMatrix(const glm::mat4 &matrix);
    GLuint getLocationPosition(void) const;
    GLuint getLocationTextureCoords(void) const;
    void activate(void);
private:
    GLuint texture;
    GLuint shaderProgram;
    glm::mat4 matrix;
};

}

#endif