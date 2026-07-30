#ifndef _CHUNK_GRID_HPP
#define _CHUNK_GRID_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <optional>

#include "fixedPackedArray.hpp"

// Chunk Grid
//
// A Chunk Grid represents a scheme for spatially dividing a 3D space into
// rectangular, axis-aligned cells. The Chunk Grid struct stores the parameters
// of the division and provides helper methods for converting between chunk
// indexes and 3D coordinates.

struct ChunkGrid
{

    glm::vec3 chunkSize;
    glm::vec3 negativeCorner;
    glm::ivec3 chunkAxesCounts;

    void init(
        const glm::vec3& negativeCorner,
        const glm::vec3& positiveCorner,
        const glm::ivec3& chunkAxesCounts
    )
    {
        this->negativeCorner = negativeCorner;
        this->chunkAxesCounts = chunkAxesCounts;
        this->chunkSize = glm::vec3(
            positiveCorner.x - negativeCorner.x / chunkAxesCounts.x,
            positiveCorner.y - negativeCorner.y / chunkAxesCounts.y,
            positiveCorner.z - negativeCorner.z / chunkAxesCounts.z
        );
    }

    std::optional<uint32_t> getChunkContaining(
        const glm::vec3& position
    ) const
    {
        glm::vec3 positionGrid = position - negativeCorner;
        if (
            positionGrid.x < 0 || positionGrid.x > chunkAxesCounts.x * chunkSize.x ||
            positionGrid.x < 0 || positionGrid.y > chunkAxesCounts.y * chunkSize.y ||
            positionGrid.x < 0 || positionGrid.z > chunkAxesCounts.z * chunkSize.z
        )
        {
            return std::nullopt;
        }
        else
        {
            uint32_t chunkX = static_cast<uint32_t>(floor(positionGrid.x / chunkSize.x));
            uint32_t chunkY = static_cast<uint32_t>(floor(positionGrid.y / chunkSize.y));
            uint32_t chunkZ = static_cast<uint32_t>(floor(positionGrid.z / chunkSize.z));
            return 
                chunkZ * chunkSize.y * chunkSize.x +
                chunkY * chunkSize.x +
                chunkX
            ;
        }
    }

    uint32_t getCount(void) const
    {
        return chunkSize.x * chunkSize.y * chunkSize.z;
    }

};

#endif