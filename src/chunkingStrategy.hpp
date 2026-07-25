#ifndef _CHUNKING_STRATEGY_HPP
#define _CHUNKING_STRATEGY_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <optional>

// Chunking Strategy
//
// A Chunking Strategy contains information and provides functions relating
// to how a world is chunked.

class ChunkingStrategy
{

private:
    
    // The side length of each cubical chunk
    float chunkSideLength_;

    // The offset between the negative-most corner of the (0, 0, 0) chunk and
    // the origin of the world. Alternatively, the position of the negative-
    // most corner of the chunk grid.
    glm::vec3 chunkOffset_;

    // The number of chunks that cover each dimension
    glm::ivec3 chunkCount_;

public:

    ChunkingStrategy(
        const float& chunkSideLength,
        const glm::vec3& bottomLeftRear,
        const glm::vec3& topRightFront
    );

    ~ChunkingStrategy(void);

    glm::ivec3 getChunk(
        const glm::vec3& position
    ) const;

    std::optional<uint> getChunkIndex(
        const glm::ivec3 chunk
    ) const;

    uint getChunkCount(void) const;

};

#endif