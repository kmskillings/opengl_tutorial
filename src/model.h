#ifndef _MODEL_H
#define _MODEL_H

#include "textures.h"

// A Model stores all the information necessary to render a single "thing."

typedef struct {
    void* vertexAttributes;
    GLuint vertexAttributesLength;
    GLuint elements;
    GLuint elementsLength;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
} Model;

Model* createModelSquareXY(
    float sideLength,   // The length of each side of the square
    Texture* texture    // The texture to apply to the square
);

void drawModel(Model* model);

#endif