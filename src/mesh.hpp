#ifndef _MESH_HPP
#define _MESH_HPP

#include "gl_includes.h"

#include <memory>
#include <vector>

namespace GlWorld
{

class Mesh
{
public:

    virtual bool has4dPosition(void) const = 0;
    virtual void bind4dPosition(GLuint location) = 0;

    virtual bool hasVertexNormals(void) const = 0;
    virtual void bindVertexNormals(GLuint location) = 0;

    virtual bool has2dTextureCoords(void) const = 0;
    virtual void bind2dTextureCoords(GLuint location) = 0;

    virtual void activate(void) = 0;
    virtual void deactivate(void) = 0;
    virtual void draw(void) = 0;

};

class MeshTextured : public Mesh
{

public:

    MeshTextured(
        const std::vector<float> &vertices,
        const std::vector<GLuint> &elements
    );

    bool has4dPosition(void) const;
    void bind4dPosition(GLuint location);

    bool hasVertexNormals(void) const;
    void bindVertexNormals(GLuint location);

    bool has2dTextureCoords(void) const;
    void bind2dTextureCoords(GLuint location);

    void activate(void);
    void deactivate(void);
    void draw(void);

    static std::shared_ptr<MeshTextured> cube(float sideLength);
    static std::shared_ptr<MeshTextured> sphere(
        float radius, 
        unsigned int segmentsVertical, 
        unsigned int segmentsHorizontal
    );

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