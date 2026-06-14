#ifndef _MESH_HPP
#define _MESH_HPP

#include <vector>

namespace GlWorld {

class Mesh {
    public:
        Mesh(
            const std::vector<float> &vertexArray,
            const std::vector<GLuint> &elementArray    
        );
        ~Mesh(void);
        void bindVertexPositionScreenSpace(GLuint index);
        void activate(void);
        void deactivate(void);
        void draw(void);
        static Mesh* Triangle(float width, float height);
        static Mesh* Rectangle(float width, float height);
    private:
        GLuint vao;
        GLuint vbo;
        GLuint ebo;
        std::vector<float> vertexArray;
        std::vector<GLuint> elementArray;
        GLuint vertexPositionScreenSpaceIndex;
};

}

#endif