#pragma once

#include <memory>
#include <utility>

#include "IPhysicsWorld.h"

class GameObject;
class Registry;

class PhysicsSystem
{
    public:
        PhysicsSystem(Registry& registry, std::unique_ptr<IPhysicsWorld> world)
            : registry(registry), world(std::move(world)) {}

        void addObject(GameObject& go) { world->addObject(go); }
        void removeObject(GameObject& go) { world->removeObject(go); }

        void update(float dt);

    private:
        static constexpr float FIXED_DT = 1.0f / 60.0f; // Fixed timestamp (60 FPS)
        static constexpr float MAX_ACCUMULATED = 0.25f; // Max accumulated time. Prevents an infinite spiral of updates

        Registry& registry;
        std::unique_ptr<IPhysicsWorld> world;
        float accumulator = 0.0f; // To check whether a step() needs to be taken
};
