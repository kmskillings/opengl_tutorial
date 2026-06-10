#include <stdlib.h>
#include <stdbool.h>

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>

#include "model.h"

void createModelSquareXY(Model* model, float sideLength)
{
    
    model->verticesAllocated = false;
    model->trianglesAllocated = false;

    // Try to allocate memory for four vertices.
    model->vertices = malloc(4 * sizeof(Vertex));
    if (model->vertices != NULL) {
        createVertex(&(model->vertices[0]), -sideLength, -sideLength, 0.0f, 0.0f);
        createVertex(&(model->vertices[1]),  sideLength, -sideLength, 1.0f, 0.0f);
        createVertex(&(model->vertices[2]), -sideLength,  sideLength, 0.0f, 1.0f);
        createVertex(&(model->vertices[3]),  sideLength,  sideLength, 1.0f, 1.0f);
        model->verticesCount = 4;
        model->verticesAllocated = true;
    }

    // Try to allocate memory for two elements.
    model->triangles = malloc(2 * sizeof(Triangle));
    if (model->triangles != NULL) {
        createTriangle(&(model->triangles[0]), 0, 1, 2);
        createTriangle(&(model->triangles[1]), 3, 2, 1);
        model->trianglesCount = 2;
        model->trianglesAllocated = true;
    }

}

void deleteModel(Model* model)
{
    if (model->verticesAllocated) {
        free (model->vertices);
    }
    if (model->trianglesAllocated) {
        free (model->triangles);
    }
}

void createVertex(Vertex* vertex, float positionX, float positionY, float texCoordX, float texCoordY)
{
    vertex->position[0] = positionX;
    vertex->position[1] = positionY;
    vertex->texCoord[0] = texCoordX;
    vertex->texCoord[1] = texCoordY;
}

void createTriangle(Triangle* triangle, GLuint indexA, GLuint indexB, GLuint indexC)
{
    triangle->indices[0] = indexA;
    triangle->indices[1] = indexB;
    triangle->indices[2] = indexC;
}