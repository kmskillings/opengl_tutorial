#ifndef _COLLISION_BROAD_SYSTEM_HPP
#define _COLLISION_BROAD_SYSTEM_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ChunkGrid;
template<typename T> struct FixedPackedArray;
struct Chunk;
struct Collision;

// Collision Broad System
//
// A Collision Broad System is responsible for broad-phase collision detection.

class CollisionBroadSystem
{

private:

public:

    // Performs broad-phase collision detection. Currently, only collisions
    // between Cami Cubes and the sphere are implemented. The successful
    // collisions are written to the collisions array.
    //
    // Broad-phase collision detection works on the chunk grid. A collision
    // is detected between the sphere and a Cami Cube iff the Cami Cube is in
    // the same chunk as the sphere or one of the 26 chunks in its neighborhood.
    void detectCollisions(
        const glm::vec3& spherePosition,
        const ChunkGrid& chunkGrid,
        const FixedPackedArray<Chunk>& chunks,
        FixedPackedArray<Collision>& collisions
    );

};

#endif