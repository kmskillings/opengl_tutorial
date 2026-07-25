#ifndef _WORLD_HPP
#define _WORLD_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

class ChunkingStrategy;

// World
//
// A World contains all the state-based data needed to process the game
// simulation. It provides this data to ComponentSystems, which are themselves
// (mostly) stateless.

class World
{

public:

    struct Chunk
    {
        // Where this chunk's contents start in a chunked array. Alternatively,
        // the cumulative count of chunks preceding this one.
        uint start;

        // How many items are in this chunk.
        uint count;

        Chunk() : start(0), count(0) {}
    };

    struct CamiCubeOrientation
    {
        float orientationAngle;
        glm::vec3 orientationAxis;
        float rotationRate;
        glm::vec3 rotationAxis;
    };

private:

    
    Chunk* chunks_;
    std::unique_ptr<ChunkingStrategy> chunkingStrategy_;

    uint camiCubeCount_;
    glm::vec3* camiCubePositions_;
    CamiCubeOrientation* camiCubeOrientations_;

    glm::vec3 spherePosition_;
    

public:

    World(
        const uint& camiCubeCount,
        const float& radius,
        const uint& averageCubesPerChunk
    );

    ~World(void);

    uint getCamiCubeCount(void) const;

    glm::vec3* getCamiCubePositions(void) const;

    CamiCubeOrientation* getCamiCubeOrientations(void) const;

    glm::vec3 getSpherePosition(void) const;
    void setSpherePosition(const glm::vec3& v);

};

#endif