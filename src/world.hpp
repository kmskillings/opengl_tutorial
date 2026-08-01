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
#include "chunkGrid.hpp"
#include "chunk.hpp"
#include "doubleBuffer.hpp"
#include "camiCubeOrientation.hpp"
#include "inputEvent.hpp"
#include "collision.hpp"
#include "instanceAttribute.hpp"

// World
//
// A World contains all the state-based data needed to process the game
// simulation. It provides this data to ComponentSystems, which are themselves
// (mostly) stateless.

class World
{

public:

    ChunkGrid chunkGrid;
    FixedPackedArray<Chunk> chunks;
    DoubleBuffer<FixedPackedArray<glm::vec3>> camiCubePositions;
    DoubleBuffer<FixedPackedArray<CamiCubeOrientation>> camiCubeOrientations;

    glm::vec3 spherePosition = glm::vec3(0.0f);
    glm::quat sphereOrientation = glm::quat(glm::vec3(0.0f));

    FixedPackedArray<InputEvent> inputEvents;

    PlayerControlState playerControlState;

    FixedPackedArray<Collision> collisionsBroad;
    DoubleBuffer<FixedPackedArray<Collision>> collisionsMedium;
    FixedPackedArray<Collision> appearedCollisions;
    FixedPackedArray<Collision> disappearedCollisions;

    FixedPackedArray<InstanceAttribute> camiCubeUpdateAttributes;
    FixedPackedArray<GLuint> camiCubeUpdateIndexes;
    FixedPackedArray<unsigned char> camiCubeUpdateData;

    void init(
        uint32_t camiCubeCount,
        float cloudRadius,
        uint32_t chunkCountAxis,
        uint32_t inputEventCountMax,
        uint32_t collisionsBroadCountMax,
        uint32_t collisionsMediumCountMax
    )
    {
        chunkGrid.init(
            glm::vec3(-cloudRadius, -cloudRadius, -cloudRadius),
            glm::vec3(cloudRadius, cloudRadius, cloudRadius),
            glm::ivec3(chunkCountAxis, chunkCountAxis, chunkCountAxis)
        );

        chunks.allocate(chunkCountAxis * chunkCountAxis * chunkCountAxis);

        camiCubePositions.a.allocate(camiCubeCount);
        camiCubePositions.b.allocate(camiCubeCount);
        camiCubeOrientations.a.allocate(camiCubeCount);
        camiCubeOrientations.b.allocate(camiCubeCount);

        inputEvents.allocate(inputEventCountMax);

        collisionsBroad.allocate(collisionsBroadCountMax);
        collisionsMedium.a.allocate(collisionsMediumCountMax);
        collisionsMedium.b.allocate(collisionsMediumCountMax);
        appearedCollisions.allocate(collisionsMediumCountMax);
        disappearedCollisions.allocate(collisionsMediumCountMax);

        camiCubeUpdateAttributes.allocate(2 * collisionsMediumCountMax);
        camiCubeUpdateIndexes.allocate(2 * collisionsMediumCountMax);
        camiCubeUpdateData.allocate(collisionsMediumCountMax * sizeof(glm::vec3));
    }

    void shutdown(void)
    {
        chunks.deallocate();
        camiCubePositions.a.deallocate();
        camiCubePositions.b.deallocate();
        camiCubeOrientations.a.deallocate();
        camiCubeOrientations.b.deallocate();
        inputEvents.deallocate();
        collisionsBroad.deallocate();
        collisionsMedium.a.deallocate();
        collisionsMedium.b.deallocate();
        camiCubeUpdateAttributes.deallocate();
        camiCubeUpdateIndexes.deallocate();
        camiCubeUpdateData.deallocate();
    }

};

#endif