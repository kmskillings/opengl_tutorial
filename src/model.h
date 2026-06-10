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
    bool        verticesAllocated;
    Triangle*   triangles;
    GLuint      trianglesCount;
    bool        trianglesAllocated;
} Model;

void createModelSquareXY(Model* model, float sideLength);
void deleteModel(Model* model);

void createVertex(Vertex* vertex, float positionX, float positionY, float texCoordX, float texCoordY);

void createTriangle(Triangle* triangle, GLuint vertexA, GLuint vertexB, GLuint vertexC);

#endif