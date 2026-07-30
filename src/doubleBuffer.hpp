#ifndef _DOUBLE_BUFFER_H
#define _DOUBLE_BUFFER_H

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "fixedPackedArray.hpp"

template<typename T>
struct DoubleBuffer
{
    
public:

    T a;
    T b;

    T* nowPtr = &a;

    T* lastPtr = &b;

    T& now(void)
    {
        return *nowPtr;
    }

    T& last(void)
    {
        return *lastPtr;
    }

    void swap(void)
    {
        T* tmp = nowPtr;
        nowPtr = lastPtr;
        lastPtr = tmp;
    }

};

#endif
