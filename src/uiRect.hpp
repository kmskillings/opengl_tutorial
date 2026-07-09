#ifndef _UI_RECT_HPP
#define _UI_RECT_HPP

#include "gl_includes.h"

#include <memory>

namespace GlWorld
{

// A UiRect represents a 2D Axis-Aligned Bounding Box on the UI. The
// UiRect does not provide any rendering functionality on its own, but
// does allow for different UI elements to be parented / nested. Ui element
// renderers can use the UiRect transform matrices to map between points in
// the UI clipping space and points in the normalized UI element's space.
class UiRect
{

private:

    // The value of anchors are stored in parent space.
    // The offets, being measured in pixels, are independent of space.
    glm::vec2 anchorBL;
    glm::vec2 anchorTR;
    glm::vec2 offsetBL;
    glm::vec2 offsetTR;

    std::weak_ptr<UiRect> parent;

public:

    enum class Space
    {
        Local,
        Parent,
        Global
    };

    UiRect(
        const glm::vec2& anchorBL,
        const glm::vec2& anchorTR,
        const glm::vec2& offsetBL,
        const glm::vec2& offsetTR
    );

    // Gets a 3x3 matrix representing a 2D affine transformation between the
    // given spaces.
    glm::mat3 getMatrix(
        Space from,
        Space to
    ) const;

    glm::vec2 getAnchorBL(void) const;
    glm::vec2 getAnchorBL(Space space) const;

    glm::vec2 getAnchorTR(void) const;
    glm::vec2 getAnchorTR(Space space) const;

    glm::vec2 getOffsetBL(void) const;

    glm::vec2 getOffsetTR(void) const;

    void setAnchorBL(const glm::vec2& anchorBL);
    void setAnchorBL(const glm::vec2& anchorBL, Space space);

    void setAnchorTR(const glm::vec2& anchorTR);
    void setAnchorTR(const glm::vec2& anchorTR, Space space);

    void setOffsetBL(const glm::vec2& offsetBL);

    void setOffsetTR(const glm::vec2& offsetTR);
    
    UiRect* setParent(std::weak_ptr<UiRect>& parent);
    UiRect* setParent(std::weak_ptr<UiRect>& parent, Space preserve);
    
    // Get the 2d scale vector where each component represents the pixels per
    // normalized unit in that dimension.
    glm::vec2 getPixelScale(void) const;

};

}

#endif