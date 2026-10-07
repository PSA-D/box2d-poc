#pragma once

#include <memory>
#include <vector>

#include "IPhysicsWorld.h"

class GameObject;

class PhysicsSystem
{
    public:
        PhysicsSystem(std::unique_ptr<IPhysicsWorld> world) : world(std::move(world)) {}

        void update(float dt);

    private:
        static const float FIXED_DT = 1.0f / 60.0f;
        static const float MAX_ACCUMULATED = 0.25f;

        std::unique_ptr<IPhysicsWorld> world;
        float accumulator = 0.0f;
};
