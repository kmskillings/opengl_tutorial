#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "chunkingStrategy.hpp"

ChunkingStrategy::ChunkingStrategy(
    const float& chunkSideLength
) :
    chunkSideLength_(chunkSideLength)
{

}

ChunkingStrategy::~ChunkingStrategy(void)
{
    
}

glm::ivec3 ChunkingStrategy::getChunk(
    const glm::vec3& position,
    const glm::vec3& origin
) const
{
    int chunkX = static_cast<int>(floor((position.x - origin.x) / chunkSideLength_));
    int chunkY = static_cast<int>(floor((position.y - origin.y) / chunkSideLength_));
    int chunkZ = static_cast<int>(floor((position.z - origin.z) / chunkSideLength_));
    return glm::ivec3(chunkX, chunkY, chunkZ);
}

uint ChunkingStrategy::getChunkIndex(
    const glm::ivec3 chunk,
    const glm::ivec3& worldSizeChunks
) const
{
    // Index is calculated with Z most significant and X least significant.
    return 
        chunk.z * worldSizeChunks.y * worldSizeChunks.x +
        chunk.y * worldSizeChunks.x +
        chunk.x
    ;
}