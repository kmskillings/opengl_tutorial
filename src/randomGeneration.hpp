#ifndef _RANDOM_GENERATION_HPP
#define _RANDOM_GENERATION_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <random>

class RandomGenerator
{

private:

    std::mt19937 engine_;
    std::uniform_real_distribution<> dist_;

public:

    RandomGenerator();

    void init(int seed);

    glm::vec3 getUnitBall(void);

    glm::vec3 getUnitSphere(void);

    float getFloat(void);

    float getPositiveFloat(void);

};

#endif