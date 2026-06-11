#include <stdlib.h>
#include <stdbool.h>

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>

#include <glfw3.h>

#include "model.h"

Model* createModelSquareXY(
    float sideLength,
    Texture* texture
)
{
    Model* model = malloc(sizeof(Model));
    if (model == NULL)
    {
        goto fail_model;
    }

    model->vertexAttributes = malloc(6 * 4 * sizeof(float));
    if (model->vertexAttributes == NULL)
    {
        goto fail_vertexAttributes;
    }
    model->vertexAttributesLength = 4;
    float s = sideLength / 2.0f;
    ((float*)model->vertexAttributes)[0]  = -s   ;  // Lower-left corner
    ((float*)model->vertexAttributes)[1]  = -s   ;
    ((float*)model->vertexAttributes)[2]  =  0.0f;
    ((float*)model->vertexAttributes)[3]  =  1.0f;
    ((float*)model->vertexAttributes)[4]  =  0.0f;
    ((float*)model->vertexAttributes)[5]  =  0.0f;
    ((float*)model->vertexAttributes)[6]  =  s   ;  // Lower-right corner
    ((float*)model->vertexAttributes)[7]  = -s   ;
    ((float*)model->vertexAttributes)[8]  =  0.0f;
    ((float*)model->vertexAttributes)[9]  =  1.0f;
    ((float*)model->vertexAttributes)[10] =  1.0f;
    ((float*)model->vertexAttributes)[11] =  0.0f;
    ((float*)model->vertexAttributes)[12] = -s   ;  // Upper-left corner
    ((float*)model->vertexAttributes)[13] =  s   ;
    ((float*)model->vertexAttributes)[14] =  0.0f;
    ((float*)model->vertexAttributes)[15] =  1.0f;
    ((float*)model->vertexAttributes)[16] =  0.0f;
    ((float*)model->vertexAttributes)[17] =  1.0f;
    ((float*)model->vertexAttributes)[18] =  s   ;  // Upper-right corner
    ((float*)model->vertexAttributes)[19] =  s   ;
    ((float*)model->vertexAttributes)[20] =  0.0f;
    ((float*)model->vertexAttributes)[21] =  1.0f;
    ((float*)model->vertexAttributes)[22] =  1.0f;
    ((float*)model->vertexAttributes)[23] =  1.0f;

    model->elements = malloc(6 * sizeof(GLuint));
    if (model->elements == NULL)
    {
        goto fail_elements;
    }
    model->elementsLength = 6;
    ((GLuint*)model->elements)[0] = 0;  // Lower left triangle
    ((GLuint*)model->elements)[1] = 1;
    ((GLuint*)model->elements)[2] = 2;
    ((GLuint*)model->elements)[3] = 3;  // Upper right triangle
    ((GLuint*)model->elements)[4] = 2;
    ((GLuint*)model->elements)[5] = 1;

    // now that all the buffers are populated, create the vao.
    glGenVertexArrays(1, &(model->vao));
    glBindVertexArray(model->vao);

    return model;

fail_elements:
    free(model->vertexAttributes);
fail_vertexAttributes:
    free(model);
fail_model:
    return NULL;
}

