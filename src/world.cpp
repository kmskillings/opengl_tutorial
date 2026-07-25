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

World::World(
    const uint& camiCubeCount,
    const float& radius,
    const uint& averageCubesPerChunk
) :
    camiCubeCount_(camiCubeCount),
    camiCubePositions_(new glm::vec3[camiCubeCount]),
    camiCubeOrientations_(new CamiCubeOrientation[camiCubeCount]),
    spherePosition_(glm::vec3(0.0f, 0.0f, 0.0f))
{

    // Based on the number of CamiCubes and the size of the ball, create the
    // chunking strategy.
    float sphereVolume = 4.0f * M_PI / 3.0f * radius * radius * radius;
    float chunkVolume = ((float)averageCubesPerChunk) / ((float)camiCubeCount) * sphereVolume;
    float chunkSideLength = cbrt(chunkVolume);
    chunkingStrategy_ = std::make_unique<ChunkingStrategy>(
        chunkSideLength,
        glm::vec3(-radius, -radius, -radius),
        glm::vec3(radius, radius, radius)
    );

    chunks_ = new Chunk[chunkingStrategy_->getChunkCount()];
    printf("Generating world with %i chunks, each with a side length of %f\n.", chunkingStrategy_->getChunkCount(), chunkSideLength);

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
        std::optional<uint> chunk = chunkingStrategy_->getChunkIndex(
            chunkingStrategy_->getChunk(position)
        );
        if (!chunk)
        {
            printf(
                "Error: Tried generating a cube outside the chubk grid at (%f, %f, %f).\n",
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
        chunks_[chunk.value_or(0)].count = chunks_[chunk.value_or(0)].count + 1;
    }

    // Go through and set each chunk's start appropriately
    uint cumulativeCount = 0;
    for(int i = 0; i < chunkingStrategy_->getChunkCount(); i++)
    {
        chunks_[i].start = cumulativeCount;
        cumulativeCount = cumulativeCount + chunks_[i].count;
    }

    // Sort the CamiCubeGenerators and parcel them out into the chunked arrays
    std::sort(
        camiCubeGenerators, 
        camiCubeGenerators + camiCubeCount_,
        [](const CamiCubeGenerator& a, const CamiCubeGenerator& b)
        {
            if (a.chunk == b.chunk)
            {
                return a.position.x < b.position.x;
            }
            return a.chunk < b.chunk;
        }
    );
    for (int i = 0; i < camiCubeCount_; i++)
    {
        camiCubePositions_[i] = camiCubeGenerators[i].position;
        camiCubeOrientations_[i] = camiCubeGenerators[i].orientation;
    }

    delete camiCubeGenerators;
}

World::~World(void)
{
    delete chunks_;
    delete camiCubePositions_;
    delete camiCubeOrientations_;
}

uint World::getCamiCubeCount(void) const
{
    return camiCubeCount_;
}

glm::vec3* World::getCamiCubePositions(void) const
{
    return camiCubePositions_;
}

World::CamiCubeOrientation* World::getCamiCubeOrientations(void) const
{
    return camiCubeOrientations_;
}

glm::vec3 World::getSpherePosition(void) const
{
    return spherePosition_;
}

void World::setSpherePosition(const glm::vec3& v)
{
    spherePosition_ = v;
}