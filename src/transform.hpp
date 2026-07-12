#ifndef _TRANSFORM_HPP
#define _TRANSFORM_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform
{

private:
    glm::vec3 position_;
    glm::quat orientation_;
    glm::vec3 scale_;

public:

    enum class Axes
    {
        Local,  // Axes that move as the transform rotates
        Fixed   // Axes that remain fixed in the transform's containing space
    };

    Transform(
        const glm::vec3& position,
        const glm::quat& orientation,
        const glm::vec3& scale
    );

    Transform(
        const glm::vec3& position,
        const float& orientationAngle,
        const glm::vec3& orientationAxis,
        const glm::vec3& scale
    );

    glm::mat4 getMatrix(void) const;
    glm::mat4 getMatrixInv(void) const;

    glm::vec3 getPosition(void) const;
    void setPosition(const glm::vec3& position);
    void translate(
        const glm::vec3& offset, 
        const Axes& axes = Axes::Fixed
    );
    void translate(
        const float& amount, 
        const glm::vec3& direction, 
        const Axes& axes = Axes::Fixed
    );

    glm::quat getOrientation(void) const;
    void setOrientation(const glm::quat& orientation);
    void setOrientation(const float& angle, const glm::vec3& axis);
    void rotate(
        const glm::quat& rotation,
        const Axes& axes = Axes::Fixed
    );
    void rotate(
        const float& angle,
        const glm::vec3& axis,
        const Axes& axes = Axes::Fixed
    );

};

#endif