#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "chunk.hpp"
#include "chunkGrid.hpp"
#include "chunkingSystem.hpp"

#include <optional>
#include <algorithm>

void ChunkingSystem::init(
    uint32_t chunkedArraySize,
    uint32_t chunkArraySize
)
{
    permutationMap_.allocate(chunkedArraySize);
    chunkCursors_.allocate(chunkArraySize);
}

void ChunkingSystem::shutdown(void)
{
    permutationMap_.deallocate();
}

void ChunkingSystem::calculateChunks(
    const ChunkGrid& chunkGrid,
    FixedPackedArray<Chunk>& chunks,
    const FixedPackedArray<glm::vec3>& positions
)
{
    chunks.count = std::min(chunks.capacity, chunkGrid.getCount());
    chunkCursors_.count = std::min(chunkCursors_.capacity, positions.count);
    permutationMap_.count = std::min(permutationMap_.capacity, positions.count);

    // Pass 1: initialize the chunks array
    for (int i = 0; i < chunks.count; i++)
    {
        chunks[i].count = 0;
        chunkCursors_[i] = 0;
    }

    for (int i = 0; i < permutationMap_.count; i++)
    {
        // This will totally break if any objects exit the chunk grid.
        // Fix this later.
        
        std::optional<uint32_t> chunkId 
            = chunkGrid.getChunkContaining(positions[i]);
        if (chunkId.has_value())
        {
            // Co-opt the permutation map to temporarily store what chunk
            // each element will go to.
            permutationMap_[i] = chunkId.value();

            chunks[chunkId.value()].count += 1;
        }
    }

    uint32_t cumulativeCount = 0;
    for (int i = 0; i < chunks.count; i++)
    {
        chunks[i].start = cumulativeCount;
        cumulativeCount += chunks[i].count;
    }

    for (int i = 0; i < permutationMap_.count; i++)
    {
        uint32_t chunkId = permutationMap_[i];
        permutationMap_[i] = chunks[chunkId].start + chunkCursors_[chunkId];
        chunkCursors_[chunkId] += 1;
    }
}