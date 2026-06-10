#include <stdlib.h>
#include <stdbool.h>

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>

#include "model.h"

bool createModelSquareXY(Model* model, float sideLength)
{
    
    // Try to allocate memory for four vertices.
    model->vertices = malloc(4 * sizeof(Vertex));
    if (model->vertices == NULL) {
        return false;
    }

    model->vertices[0].position[0] = -sideLength;
    model->vertices[0].position[1] = -sideLength;

    model->vertices[1].position[0] =  sideLength;
    model->vertices[1].position[1] = -sideLength;

    model->vertices[2].position[0] = -sideLength;
    model->vertices[2].position[1] =  sideLength;

    model->vertices[3].position[0] =  sideLength;
    model->vertices[3].position[1] =  sideLength;

    model->vertices[0].texCoord[0] = 0.0f;
    model->vertices[0].texCoord[1] = 0.0f;

    model->vertices[0].texCoord[0] = 1.0f;
    model->vertices[0].texCoord[1] = 0.0f;

    model->vertices[0].texCoord[0] = 0.0f;
    model->vertices[0].texCoord[1] = 1.0f;

    model->vertices[0].texCoord[0] = 1.0f;
    model->vertices[0].texCoord[1] = 1.0f;

    model->verticesCount = 4;

    // Try to allocate memory for two elements.
    model->elements = malloc(2 * sizeof(Triangle));
    if (model->elements == NULL) {
        free (model->elements);
        return false;
    }

    model->elements[0].indices[0] = 0;
    model->elements[0].indices[1] = 1;
    model->elements[0].indices[2] = 2;

    model->elements[1].indices[0] = 3;
    model->elements[1].indices[1] = 2;
    model->elements[1].indices[2] = 1;

    model->elementsCount = 2;

    return true;

}

void deleteModel(Model* model)
{
    free (model->vertices);
    free (model->vertices);
}