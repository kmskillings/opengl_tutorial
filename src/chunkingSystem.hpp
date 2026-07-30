#ifndef _CHUNKING_SYSTEM_HPP
#define _CHUNKING_SYSTEM_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <optional>

#include "fixedPackedArray.hpp"
#include "doubleBuffer.hpp"

struct ChunkGrid;
struct Chunk;

class ChunkingSystem
{

private:

    // The element at last[key] goes to now[value].
    FixedPackedArray<uint32_t> permutationMap_;
    FixedPackedArray<uint32_t> chunkCursors_;

public:

    void init(
        uint32_t chunkedArraySize,
        uint32_t chunkArraySize
    );

    void shutdown(void);

    void calculateChunks(
        const ChunkGrid& chunkGrid,
        FixedPackedArray<Chunk>& chunks,
        const FixedPackedArray<glm::vec3>& positions
    );

    template<typename T>
    void chunkArray(
        DoubleBuffer<FixedPackedArray<T>>& array
    )
    {
        array.swap();
        for (int i = 0; i < permutationMap_.count; i++)
        {
            array.now()[permutationMap_[i]] = array.last()[i];
        }
    }

};

#endif