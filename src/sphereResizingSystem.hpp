#ifndef _SPHERE_RESIZING_SYSTEM_HPP
#define _SPHERE_RESIZING_SYSTEM_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "inputEvent.hpp"

template<typename T>
struct FixedPackedArray;

class SphereResizingSystem
{

private:

    float minSize_;
    float maxSize_;
    float resizingSensitivity_;

public:

    SphereResizingSystem(void);

    SphereResizingSystem(
        float minSize,
        float maxSize,
        float resizingSensitivity
    );

    void resizeSphere(
        const FixedPackedArray<InputEvent>& inputEvents,
        float& sphereRadius
    ) const;

};

#endif