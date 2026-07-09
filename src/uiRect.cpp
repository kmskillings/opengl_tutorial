#include "uiRect.hpp"

#include "gl_includes.h"

#include <memory>

namespace GlWorld
{

UiRect::UiRect(
    const glm::vec2& anchorBL,
    const glm::vec2& anchorTR,
    const glm::vec2& offsetBL,
    const glm::vec2& offsetTR
) :
    anchorBL(anchorBL),
    anchorTR(anchorTR),
    offsetBL(offsetBL),
    offsetTR(offsetTR),
    parent(std::weak_ptr<UiRect>())
{

}

glm::mat3 UiRect::getMatrix(
    UiRect::Space from,
    UiRect::Space to
) const
{
    if (from == to)
    {
        return glm::identity<glm::mat3>();
    }

    else if (
        from == UiRect::Space::Local && 
        to == UiRect::Space::Parent
    )
    {
        glm::vec2 m = 
            this->anchorTR - this->anchorBL +
            1.0f / this->getPixelScale() * (
                this->offsetTR - this->offsetBL
            )
        ;

        glm::vec2 b =
            this->anchorBL + this->anchorTR +
            1.0f / this->getPixelScale() * (
                this->offsetBL + this->offsetTR
            )
        ;

        // Transpose is there so that matrix "looks right," since
        // the mat3 constructor expects the elements to be passed
        // column-wise.
        glm::mat3 matrix = glm::transpose(glm::mat3(
            0.5f * m.x,     0.0f,       0.5 * b.x,
            0.0f,           0.5f * m.y, 0.5 * b.y,
            0.0f,           0.0f,       1.0f
        ));
        return matrix;
    }

    else if (
        from == UiRect::Space::Parent 
        && to == UiRect::Space::Global
    )
    {
        if (this->parent.expired())
        {
            return this->getMatrix(
                UiRect::Space::Global,
                UiRect::Space::Global
            );
        }
        else
        {
            return this->parent.lock()->getMatrix(
                UiRect::Space::Local,
                UiRect::Space::Global
            );
        }
    }

    else if (
        from == UiRect::Space::Local 
        && to == UiRect::Space::Global
    )
    {
        return this->getMatrix(
            UiRect::Space::Parent, 
            UiRect::Space::Global
        )
        * this->getMatrix(
            UiRect::Space::Local, 
            UiRect::Space::Parent
        );
    }

    else
    {
        return glm::inverse(this->getMatrix(to, from));
    }
}

glm::vec2 UiRect::getAnchorBL(void) const
{
    return this->getAnchorBL(UiRect::Space::Global);
}

glm::vec2 UiRect::getAnchorBL(UiRect::Space space) const
{
    return
        this->getMatrix(UiRect::Space::Parent, space) *
        glm::vec3(this->anchorBL, 1.0f)
    ;
}

glm::vec2 UiRect::getAnchorTR(void) const
{
    return this->getAnchorTR(UiRect::Space::Global);
}

glm::vec2 UiRect::getAnchorTR(UiRect::Space space) const
{
    return
        this->getMatrix(UiRect::Space::Parent, space) *
        glm::vec3(this->anchorTR, 1.0f)
    ;
}

glm::vec2 UiRect::getOffsetBL(void) const
{
    return this->offsetBL;
}

glm::vec2 UiRect::getOffsetTR(void) const
{
    return this->offsetTR;
}

void UiRect::setAnchorBL(const glm::vec2& anchorBL)
{
    this->setAnchorBL(anchorBL, UiRect::Space::Global);
}

void UiRect::setAnchorBL(
    const glm::vec2& anchorBL,
    UiRect::Space space
)
{
    this->anchorBL = 
        this->getMatrix(space, UiRect::Space::Parent) *
        glm::vec3(anchorBL, 1.0f)
    ;
}

void UiRect::setAnchorTR(const glm::vec2& anchorTR)
{
    this->setAnchorTR(anchorTR, UiRect::Space::Global);
}

void UiRect::setAnchorTR(
    const glm::vec2& anchorTR,
    UiRect::Space space
)
{
    this->anchorTR = 
        this->getMatrix(space, UiRect::Space::Parent) *
        glm::vec3(anchorTR, 1.0f)
    ;
}

void UiRect::setOffsetBL(const glm::vec2& offsetBL)
{
    this->offsetBL = offsetBL;
}

void UiRect::setOffsetTR(const glm::vec2& offsetTR)
{
    this->offsetTR = offsetTR;
}

UiRect* UiRect::setParent(std::weak_ptr<UiRect>& parent)
{
    return this->setParent(parent, UiRect::Space::Global);
}

UiRect* UiRect::setParent(
    std::weak_ptr<UiRect>& parent, 
    UiRect::Space preserve
)
{
    glm::vec2 newAnchorBL = this->getAnchorBL(preserve);
    glm::vec2 newAnchorTR = this->getAnchorTR(preserve);

    UiRect* oldParent = this->parent.lock().get();
    this->parent = std::move(parent);

    this->setAnchorBL(newAnchorBL, preserve);
    this->setAnchorTR(newAnchorTR, preserve);

    return oldParent;
}

glm::vec2 UiRect::getPixelScale(void) const
{
    glm::vec2 parentPixelScale;
    if (this->parent.expired())
    {
        parentPixelScale = glm::vec2(0.0f, 0.0f);
    }
    else
    {
        parentPixelScale = this->parent.lock()->getPixelScale();
    }

    glm::vec2 sizePixels = 
        this->getOffsetTR() - this->getAnchorBL() +
        parentPixelScale * (
            this->getAnchorTR() - this->getAnchorBL()
        )
    ;

    return 0.5f * sizePixels;
}

}