#ifndef _MESH_HPP
#define _MESH_HPP

namespace GlWorld {

class Mesh {
    public:
        Mesh(void) {
            glGenVertexArrays(1, &this->vao);
            glBindVertexArray(this->vao);
            glGenBuffers(1, &this->vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertexArray), this->vertexArray, GL_STATIC_DRAW);
        }
        void bindVertexPositionScreenSpace(GLuint index)
        {
            this->vertexPositionScreenSpaceIndex = index;
            glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, 0);
        }
        void activate(void)
        {
            glEnableVertexArrayAttrib(this->vao, this->vertexPositionScreenSpaceIndex);
        }
        void deactivate(void)
        {
            glDisableVertexArrayAttrib(this->vao, this->vertexPositionScreenSpaceIndex);
        }
        void draw(void)
        {
            glDrawArrays(GL_TRIANGLES, 0, sizeof(this->vertexArray) / sizeof(this->vertexArray[0]));
        }
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