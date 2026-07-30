#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "randomGeneration.hpp"

#include <random>

RandomGenerator::RandomGenerator() :
    dist_(-1.0f, 1.0f)
{}

void RandomGenerator::init(int seed)
{
    engine_.seed(seed);
}

glm::vec3 RandomGenerator::getUnitBall(void)
{
    glm::vec3 v;
    while (true)
    {
        v.x = getFloat();
        v.y = getFloat();
        v.z = getFloat();
        if (glm::length(v) <= 1.0f)
        {
            return v;
        }
    }
}

glm::vec3 RandomGenerator::getUnitSphere(void)
{
    glm::vec3 v = getUnitBall();
    v = glm::normalize(v);
    return v;
}

float RandomGenerator::getFloat(void)
{
    return dist_(engine_);
}

float RandomGenerator::getPositiveFloat(void)
{
    float f = getFloat();
    if (f < 0.0f)
    {
        return -f;
    }
    else
    {
        return f;
    }
}