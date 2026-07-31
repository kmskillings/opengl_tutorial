#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "collisionBroadSystem.hpp"

#include "chunkGrid.hpp"
#include "fixedPackedArray.hpp"
#include "chunk.hpp"
#include "collision.hpp"

void CollisionBroadSystem::detectCollisions(
    const glm::vec3& spherePosition,
    const ChunkGrid& chunkGrid,
    const FixedPackedArray<Chunk>& chunks,
    FixedPackedArray<Collision>& collisions
)
{
    collisions.clear();

    glm::ivec3 sphereChunkCoord
        = chunkGrid.getChunkCoordContaining(spherePosition);

    for (int i = 0; i < 27; i++)
    {
        int32_t chunkOffsetZ = i / 9 - 1;
        int32_t chunkOffsetY = (i % 9) / 3 - 1;
        int32_t chunkOffsetX = i % 3 - 1;
        glm::ivec3 chunkOffset = glm::ivec3(
            chunkOffsetX, 
            chunkOffsetY, 
            chunkOffsetZ
        );
        std::optional<int32_t> chunkIndex 
            = chunkGrid.getChunkIndex(sphereChunkCoord + chunkOffset);
        if (chunkIndex.has_value())
        {
            const Chunk& chunk = chunks[chunkIndex.value()];
            for (int i = 0; i < chunk.count; i++)
            {
                bool success = collisions.push(Collision(chunk.start + i));
                if (!success)
                {
                    return;
                }
            }
        }
    }
}