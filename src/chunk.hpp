#ifndef _CHUNK_HPP
#define _CHUNK_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Chunk
{
    // Where this chunk's contents start in a chunked array. Alternatively,
    // the cumulative count of chunks preceding this one.
    uint start;

    // How many items are in this chunk.
    uint count;

    Chunk() : start(0), count(0) {}
};

#endif