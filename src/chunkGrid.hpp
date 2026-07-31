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
            (positiveCorner.x - negativeCorner.x) / chunkAxesCounts.x,
            (positiveCorner.y - negativeCorner.y) / chunkAxesCounts.y,
            (positiveCorner.z - negativeCorner.z) / chunkAxesCounts.z
        );
    }

    std::optional<uint32_t> getChunkContaining(
        const glm::vec3& position
    ) const
    {
        glm::ivec3 chunkCoord = getChunkCoordContaining(position);
        return getChunkIndex(chunkCoord);
    }

    std::optional<uint32_t> getChunkIndex(
        const glm::ivec3 chunkCoord
    ) const
    {
        
        if (
            chunkCoord.x < 0 || chunkCoord.x > chunkAxesCounts.x ||
            chunkCoord.y < 0 || chunkCoord.y > chunkAxesCounts.y ||
            chunkCoord.z < 0 || chunkCoord.z > chunkAxesCounts.z
        )
        {
            return std::nullopt;
        }
        
        return 
            chunkCoord.z * chunkAxesCounts.y * chunkAxesCounts.x +
            chunkCoord.y * chunkAxesCounts.x +
            chunkCoord.x
        ;
    }

    glm::ivec3 getChunkCoordContaining(
        const glm::vec3 position
    ) const
    {
        glm::vec3 positionGrid = position - negativeCorner;
        int32_t chunkX = static_cast<int32_t>(floor(positionGrid.x / chunkSize.x));
        int32_t chunkY = static_cast<int32_t>(floor(positionGrid.y / chunkSize.y));
        int32_t chunkZ = static_cast<int32_t>(floor(positionGrid.z / chunkSize.z));
        return glm::ivec3(chunkX, chunkY, chunkZ);
    }

    uint32_t getCount(void) const
    {
        return chunkAxesCounts.x * chunkAxesCounts.y * chunkAxesCounts.z;
    }

};

#endif