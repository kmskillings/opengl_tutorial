#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>

#include "world.hpp"
#include "chunkingStrategy.hpp"
#include "randomGeneration.hpp"
#include "chunk.hpp"
#include "inputEvent.hpp"

World::World(
    const uint& camiCubeCount,
    const float& radius,
    const uint& averageCubesPerChunk
)
{

    camiCubePositions.allocate(camiCubeCount);
    camiCubeOrientations.allocate(camiCubeCount);

    // Based on the number of CamiCubes and the size of the ball, create the
    // chunking strategy.
    float sphereVolume = 4.0f * M_PI / 3.0f * radius * radius * radius;
    float chunkVolume = ((float)averageCubesPerChunk) / ((float)camiCubeCount) * sphereVolume;
    float chunkSideLength = cbrt(chunkVolume);
    chunkingStrategy = std::make_unique<ChunkingStrategy>(
        chunkSideLength,
        glm::vec3(-radius, -radius, -radius),
        glm::vec3(radius, radius, radius)
    );

    chunks.allocate(chunkingStrategy->getChunkCount());
    printf("Generating world with %i chunks, each with a side length of %f\n.", chunks.capacity, chunkSideLength);
    for(int i = 0; i < chunks.capacity; i++)
    {
        chunks.push(Chunk());
    }

    // Generate the cubes. They're not sorted yet, but set up to do so later.
    struct CamiCubeGenerator
    {
        uint chunk;
        glm::vec3 position;
        CamiCubeOrientation orientation;
    };
    CamiCubeGenerator* camiCubeGenerators = new CamiCubeGenerator[camiCubeCount];
    RandomGenerator randomGenerator(11141997);
    for (uint i = 0; i < camiCubeCount; i++)
    {
        glm::vec3 position = randomGenerator.getUnitBall() * radius;
        float orientationAngle = randomGenerator.getPositiveFloat() * M_PI;
        glm::vec3 orientationAxis = randomGenerator.getUnitSphere();
        glm::quat orientation = glm::angleAxis(orientationAngle, orientationAxis);
        float rotationRate = randomGenerator.getPositiveFloat();
        glm::vec3 rotationAxis = randomGenerator.getUnitSphere();
        std::optional<uint> chunk = chunkingStrategy->getChunkIndex(
            chunkingStrategy->getChunk(position)
        );
        if (!chunk)
        {
            printf(
                "Error: Tried generating a cube outside the chunk grid at (%f, %f, %f).\n",
                position.x,
                position.y,
                position.z
            );
        }
        camiCubeGenerators[i] = {
            chunk.value_or(0),
            position,
            {
                orientationAngle,
                orientationAxis,
                rotationRate,
                rotationAxis
            }
        };

        // Increment the appropriate chunk's count
        chunks[chunk.value_or(0)].count = chunks[chunk.value_or(0)].count + 1;
    }

    // Go through and set each chunk's start appropriately
    uint cumulativeCount = 0;
    for(int i = 0; i < chunks.count; i++)
    {
        chunks[i].start = cumulativeCount;
        cumulativeCount = cumulativeCount + chunks[i].count;
    }

    // Sort the CamiCubeGenerators and parcel them out into the chunked arrays
    std::sort(
        camiCubeGenerators, 
        camiCubeGenerators + camiCubeCount,
        [](const CamiCubeGenerator& a, const CamiCubeGenerator& b)
        {
            if (a.chunk == b.chunk)
            {
                return a.position.x < b.position.x;
            }
            return a.chunk < b.chunk;
        }
    );
    for (int i = 0; i < camiCubeCount; i++)
    {
        camiCubePositions.push(camiCubeGenerators[i].position);
        camiCubeOrientations.push(camiCubeGenerators[i].orientation);
    }

    delete camiCubeGenerators;

    inputEvents.allocate(10);
}

World::~World(void)
{
    chunks.deallocate();
    camiCubeOrientations.deallocate();
    camiCubePositions.deallocate();
}