#pragma once

#include "Types.h"

struct Transform : Component
{
    Vec2  position {0, 0};
    float rotation {0}; // In radians
};
