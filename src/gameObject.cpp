#include "gameObject.hpp"

namespace GlWorld {

void GameObject::drawRgb(void) const
{
    this->model->drawRgb();
}

}