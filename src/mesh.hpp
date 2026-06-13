#ifndef _MESH_HPP
#define _MESH_HPP

#include <vector>

namespace GlWorld {

class Mesh {
    public:
        Mesh(const std::vector<float> &vertexArray);
        ~Mesh(void);
        void bindVertexPositionScreenSpace(GLuint index);
        void activate(void);
        void deactivate(void);
        void draw(void);
        static Mesh* Triangle(float width, float height);
    private:
        GLuint vao;
        GLuint vbo;
        std::vector<float> vertexArray;
        GLuint vertexPositionScreenSpaceIndex;
};

}

#endif