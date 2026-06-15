#ifndef _MODEL_HPP
#define _MODEL_HPP

#include <memory>

#include "gl_includes.h"

namespace GlWorld {

class Model {
public:
    virtual void draw(
        const glm::mat4 &matrixModel,
        const glm::mat4 &matrixView,
        const glm::mat4 &matrixProject 
    ) const = 0;
private:
};

}

#endif