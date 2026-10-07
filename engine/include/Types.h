#pragma once

struct Vec2 {
    float x, y;
};

struct Transform
{
    Vec2  position {0, 0};
    float rotation {0};
};
