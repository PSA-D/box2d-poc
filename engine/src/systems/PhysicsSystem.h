#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "IPhysicsWorld.h"

class GameObject;

class PhysicsSystem
{
    public:
        PhysicsSystem(std::unique_ptr<IPhysicsWorld> world) : world(std::move(world)) {}

        void addObject(GameObject& go) { world->addObject(go); }
        void removeObject(GameObject& go) { world->removeObject(go); }

        void update(float dt);

    private:
        static constexpr float FIXED_DT = 1.0f / 60.0f;
        static constexpr float MAX_ACCUMULATED = 0.25f;

        std::unique_ptr<IPhysicsWorld> world;
        float accumulator = 0.0f;
};
