#ifndef _HIGHLIGHT_SYSTEM_HPP
#define _HIGHLIGHT_SYSTEM_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <optional>

class ChunkingStrategy;
template<typename T>
struct FixedPackedArray;
struct Chunk;

class HighlightSystem
{

private:

public:

    void getHighlights(
        const glm::vec3& spherePosition,
        const std::optional<uint>& sphereChunkIndex,
        const ChunkingStrategy& chunkingStrategy,
        const FixedPackedArray<Chunk>& chunks,
        FixedPackedArray<uint>& highlightedIndices
    );

};

#endif
