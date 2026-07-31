#ifndef _COLLISION_HPP
#define _COLLISION_HPP

// Right now, since collisions are only detected between the sphere and Cami
// Cubes, and there's only one sphere, a Collision is fully defined by the
// index of the Cami Cube.
struct Collision
{

    uint32_t camiCubeIndex;

    Collision(void) :
        camiCubeIndex(0)
    {}

    Collision(
        uint32_t camiCubeIndex
    ) :
        camiCubeIndex(camiCubeIndex)
    {}

};

#endif