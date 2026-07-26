#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "highlightSystem.hpp"

#include "chunkingStrategy.hpp"
#include "world.hpp"
#include "fixedPackedArray.hpp"
#include "chunk.hpp"

void HighlightSystem::getHighlights(
    const glm::vec3& spherePosition,
    const std::optional<uint>& sphereChunkIndex,
    const ChunkingStrategy& chunkingStrategy,
    const FixedPackedArray<Chunk>& chunks,
    FixedPackedArray<uint>& highlightedIndices
)
{
    if (!sphereChunkIndex)
    {
        return;
    }

    Chunk sphereChunk = chunks[sphereChunkIndex.value()];
    for (int i = 0; i < sphereChunk.count; i++)
    {
        highlightedIndices.push(sphereChunk.start + i);
    }
}