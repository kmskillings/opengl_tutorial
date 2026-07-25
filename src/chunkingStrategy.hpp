#ifndef _CHUNKING_STRATEGY_HPP
#define _CHUNKING_STRATEGY_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Chunking Strategy
//
// A Chunking Strategy contains information and provides functions relating
// to how a world is chunked.

class ChunkingStrategy
{

private:
    
    // The side length of each cubical chunk
    float chunkSideLength_;

public:

    ChunkingStrategy(
        const float& chunkSideLength
    );

    ~ChunkingStrategy(void);

    glm::ivec3 getChunk(
        const glm::vec3& position,
        const glm::vec3& origin = glm::vec3(0.0f, 0.0f, 0.0f) // The negative-most corner of the world
    ) const;

    uint getChunkIndex(
        const glm::ivec3 chunk,
        const glm::ivec3& worldSizeChunks
    ) const;

};

#endif