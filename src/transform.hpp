#ifndef _TRANSFORM_HPP
#define _TRANSFORM_HPP

#include "gl_includes.h"

#include <memory>
#include <unordered_set>

namespace GlWorld
{

enum class TransformAxes
{
    Local,
    Parent,
    Global
};
 
class Transform
{
public:

    Transform(void);

    glm::mat4 getMatrixModel(void) const;
    glm::mat4 getMatrixModel(TransformAxes axes) const;

    glm::mat4 getMatrixModelInv(void) const;
    glm::mat4 getMatrixModelInv(TransformAxes axes) const;

    glm::vec3 getPosition(void) const;
    glm::vec3 getPosition(TransformAxes axes) const;

    void setPosition(const glm::vec3 &position);
    void setPosition(const glm::vec3 &position, TransformAxes axes);

    void translate(const glm::vec3 &offset);
    void translate(const glm::vec3 &offset, TransformAxes axes);
    void translate(const glm::vec3 &direction, float distance);
    void translate(const glm::vec3 &direction, float distance, TransformAxes axes);

    glm::quat getRotation(void) const;
    glm::quat getRotation(TransformAxes axes) const;

    void setRotation(const glm::quat &rotation);
    void setRotation(const glm::quat &rotation, TransformAxes axes);
    void setRotation(float angle, const glm::vec3 axis);
    void setRotation(float angle, const glm::vec3 axis, TransformAxes axes);

    void rotate(const glm::quat &rotation);
    void rotate(const glm::quat &rotation, TransformAxes axes);
    void rotate(float angle, const glm::vec3 axis);
    void rotate(float angle, const glm::vec3 axis, TransformAxes axes);

    glm::vec3 getScale(void) const;

    void setScale(const glm::vec3 &scale);

    void scaleBy(const glm::vec3 &scale);

    Transform* getParent(void) const;

    void setParent(Transform* parent);
    void setParent(Transform* parent, TransformAxes preserve);

    static const Transform& zero(void);

private:

    std::unordered_set<Transform*> getParents(void);

    glm::vec3 position; // Stored in parent space.
    glm::quat rotation;
    glm::vec3 scale;
    Transform* parent;
    static Transform _zero;
};

class HasTransform
{
public:
    virtual Transform* getTransform(void) const = 0;
};

}


#endif