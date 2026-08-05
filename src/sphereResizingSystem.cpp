#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "sphereResizingSystem.hpp"

#include "inputEvent.hpp"
#include "fixedPackedArray.hpp"

SphereResizingSystem::SphereResizingSystem(void) :
    resizingSensitivity_(0.0f)
{}

SphereResizingSystem::SphereResizingSystem(
    float minSize,
    float maxSize,
    float resizingSensitivity
) :
    minSize_(minSize),
    maxSize_(maxSize),
    resizingSensitivity_(resizingSensitivity)
{}

void SphereResizingSystem::resizeSphere(
    const FixedPackedArray<InputEvent>& inputEvents,
    float& sphereRadius
) const
{
    for (int i = 0; i < inputEvents.count; i++)
    {
        const InputEvent& inputEvent = inputEvents[i];
        if (!std::holds_alternative<ScrollwheelEvent>(inputEvent))
        {
            continue;
        }

        const ScrollwheelEvent& swEvent = std::get<ScrollwheelEvent>(inputEvent);
        sphereRadius = sphereRadius * 
            exp(resizingSensitivity_ * swEvent.scrollAmount);
        if (sphereRadius > maxSize_)
        {
            sphereRadius = maxSize_;
        }
        else if (sphereRadius < minSize_)
        {
            sphereRadius = minSize_;
        }
    }
}