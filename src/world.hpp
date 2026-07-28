#ifndef _WORLD_HPP
#define _WORLD_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <optional>

#include "fixedPackedArray.hpp"
#include "playerControlState.hpp"

class ChunkingStrategy;
struct Chunk;
struct InputEvent;

// World
//
// A World contains all the state-based data needed to process the game
// simulation. It provides this data to ComponentSystems, which are themselves
// (mostly) stateless.

class World
{

public:

    struct CamiCubeOrientation
    {
        float orientationAngle;
        glm::vec3 orientationAxis;
        float rotationRate;
        glm::vec3 rotationAxis;
    };

    FixedPackedArray<Chunk> chunks;
    FixedPackedArray<glm::vec3> camiCubePositions;
    FixedPackedArray<CamiCubeOrientation> camiCubeOrientations;

    std::unique_ptr<ChunkingStrategy> chunkingStrategy;

    glm::vec3 spherePosition = glm::vec3(0.0f);
    std::optional<uint> sphereChunkIndex;
    glm::quat sphereOrientation = glm::quat(glm::vec3(0.0f));

    FixedPackedArray<InputEvent> inputEvents;

    PlayerControlState playerControlState;

    World(
        const uint& camiCubeCount,
        const float& radius,
        const uint& averageCubesPerChunk
    );

    ~World(void);

};

#endif