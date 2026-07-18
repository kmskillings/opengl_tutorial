#ifndef _MESH_SPHERE_HPP
#define _MESH_SPHERE_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class MeshSphere
{

private:

    GLuint vao_;
    GLuint vbo_;
    GLuint eboTriangles_;
    uint countTriangles_;
    GLuint eboLines_;
    uint countLines_;

public:

    MeshSphere(
        uint segmentsVertical,
        uint segmentsHorizontal,
        float radius
    );

    GLuint getVao(void) const;
    
    GLuint getEboTriangles(void) const;
    uint getCountTriangles(void) const;

    GLuint getEboLines(void) const;
    uint getCountLines(void) const;

};

#endif