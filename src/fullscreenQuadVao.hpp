#ifndef _FULLSCREEN_QUAD_VAO_HPP
#define _FULLSCREEN_QUAD_VAO_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct FullscreenQuadVao
{

public:

    GLuint vao;
    GLuint vbo;
    GLuint eboTriangles;

    FullscreenQuadVao(void)
    {
        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        float vertices[] = {
            -1.0f, -1.0f,
             1.0f, -1.0f,
            -1.0f,  1.0f,
             1.0f,  1.0f
        };
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(vertices),
            vertices,
            GL_STATIC_DRAW
        );
        glEnableVertexArrayAttrib(vao, 0);
        glVertexAttribPointer(
            0,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::vec2),
            (void*)0
        );
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &eboTriangles);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboTriangles);
        uint elements[] = {
            0, 1, 2,
            3, 2, 1
        };
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(elements),
            elements,
            GL_STATIC_DRAW
        );
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

};

#endif