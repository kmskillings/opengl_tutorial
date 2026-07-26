#ifndef _FIXED_PACKED_ARRAY
#define _FIXED_PACKED_ARRAY

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

template<typename T>
struct FixedPackedArray
{
    uint32_t capacity;
    uint32_t count = 0;
    T* data = nullptr;

    void allocate(uint32_t capacity)
    {
        this->capacity = capacity;
        this->count = 0;
        this->data = static_cast<T*>(std::malloc(capacity * sizeof(T)));
    }

    void deallocate(void)
    {
        if (this->data != nullptr)
        {
            free(this->data);
            this->data = nullptr;
        }
        this->capacity = 0;
        this->count = 0;
    }

    bool push(const T& v)
    {
        if (this->count < this->capacity)
        {
            this->data[this->count] = v;
            this->count = this->count + 1;
            return true;
        }
        else
        {
            return false;
        }
    }

    void clear(void)
    {
        this->count = 0;
    }

    T& operator[](uint32_t index) 
    { 
        return data[index]; 
    }

    const T& operator[](uint32_t index) const 
    { 
        return data[index]; 
    }

};

#endif