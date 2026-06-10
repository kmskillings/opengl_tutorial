#ifndef _MODEL_H
#define _MODEL_H

#include <stdbool.h>

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>

#include <cglm/cglm.h>

typedef struct {
    vec2 position;
    vec2 texCoord;
} Vertex;

typedef struct {
    GLuint indices[3];
} Triangle;

typedef struct {
    Vertex*     vertices;
    GLuint      verticesCount;
    Triangle*   elements;
    GLuint      elementsCount;
} Model;

bool createModelSquareXY(Model* model, float sideLength);
void deleteModel(Model* model);

#endif