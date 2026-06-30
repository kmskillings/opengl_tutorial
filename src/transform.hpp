#ifndef _TRANSFORM_HPP
#define _TRANSFORM_HPP

#include "gl_includes.h"

#include <memory>

namespace GlWorld
{
 
class Transform
{
public:

    Transform(void);

    glm::mat4 getMatrixModel(void) const;

    glm::mat4 getMatrixModelInv(void) const;

    glm::vec3 getPosition(void) const;

    void setPosition(const glm::vec3 &position);

    void translate(const glm::vec3 &offset);

    void translate(const glm::vec3 &direction, float distance);

    glm::quat getRotation(void) const;

    void setRotation(const glm::quat &rotation);

    void setRotation(float angle, const glm::vec3 axis);

    void rotate(const glm::quat &rotation);

    void rotate(float angle, const glm::vec3 axis);

    glm::vec3 getScale(void) const;

    void setScale(const glm::vec3 &scale);

    void scaleBy(const glm::vec3 &scale);

private:
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
};

class HasTransform
{
public:
    virtual std::shared_ptr<Transform> getTransform(void) const = 0;
};

}


#endif