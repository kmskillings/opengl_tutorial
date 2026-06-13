#ifndef _MESH_HPP
#define _MESH_HPP

namespace GlWorld {

class Mesh {
    public:
        Mesh(void);
        void bindVertexPositionScreenSpace(GLuint index);
        void activate(void);
        void deactivate(void);
        void draw(void);
    private:
        GLuint vao;
        GLuint vbo;
        float vertexArray[6] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.0f,  0.5
        };
        GLuint vertexPositionScreenSpaceIndex;
};

}

#endif