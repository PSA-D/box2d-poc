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
        static constexpr float FIXED_DT = 1.0f / 60.0f;
        static constexpr float MAX_ACCUMULATED = 0.25f;

        Registry& registry;
        std::unique_ptr<IPhysicsWorld> world;
        float accumulator = 0.0f;
};
