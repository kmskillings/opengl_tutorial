#ifndef _HAS_TRANSFORM_HPP
#define _HAS_TRANSFORM_HPP

#include "transform.hpp"
#include <memory>

namespace GlWorld
{

class HasTransform
{
    public:
        virtual std::shared_ptr<Transform> getTransform(void) const = 0;
};

}

#endif