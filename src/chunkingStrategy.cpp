#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <optional>

#include "chunkingStrategy.hpp"

ChunkingStrategy::ChunkingStrategy(
    const float& chunkSideLength,
    const glm::vec3& bottomLeftRear,
    const glm::vec3& topRightFront
) :
    chunkSideLength_(chunkSideLength),
    chunkOffset_(bottomLeftRear),
    chunkCount_(glm::ivec3(
        static_cast<int>(ceil((topRightFront.x - bottomLeftRear.x) / chunkSideLength)),
        static_cast<int>(ceil((topRightFront.y - bottomLeftRear.y) / chunkSideLength)),
        static_cast<int>(ceil((topRightFront.z - bottomLeftRear.z) / chunkSideLength))
    ))
{

}

ChunkingStrategy::~ChunkingStrategy(void)
{

}

glm::ivec3 ChunkingStrategy::getChunk(
    const glm::vec3& position
) const
{
    int chunkX = static_cast<int>(floor((position.x - chunkOffset_.x) / chunkSideLength_));
    int chunkY = static_cast<int>(floor((position.y - chunkOffset_.y) / chunkSideLength_));
    int chunkZ = static_cast<int>(floor((position.z - chunkOffset_.z) / chunkSideLength_));
    return glm::ivec3(chunkX, chunkY, chunkZ);
}

std::optional<uint> ChunkingStrategy::getChunkIndex(
    const glm::ivec3 chunk
) const
{
    if (
        chunk.x < 0 || chunk.x >= chunkCount_.x ||
        chunk.y < 0 || chunk.y >= chunkCount_.y ||
        chunk.z < 0 || chunk.z >= chunkCount_.z
    )
    {
        return std::nullopt;
    }
    // Index is calculated with Z most significant and X least significant.
    return 
        chunk.z * chunkCount_.y * chunkCount_.x +
        chunk.y * chunkCount_.x +
        chunk.x
    ;
}

uint ChunkingStrategy::getChunkCount(void) const
{
    return chunkCount_.x * chunkCount_.y * chunkCount_.z;
}