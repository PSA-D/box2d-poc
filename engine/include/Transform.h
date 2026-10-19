#pragma once

#include "Types.h"

class Transform : public Component
{
    public:
        Transform();
        Transform(const Vec2& pos, float rot = 0.0f) : position(pos), rotation(rot) {};

        void setPosition(const Vec2& pos) { position.x = pos.x; position.y = pos.y; }
        Vec2 getPosition() const { return position; }

        void setRotation(float rot) { rotation = rot; }
        float getRotation() const { return rotation; }
    private:
        Vec2  position {0, 0};
        float rotation {0}; // In radians
};
