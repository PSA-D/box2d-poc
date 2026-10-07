#include "PhysicsSystem.h"

#include <algorithm>

#include "../../include/Types.h"
#include "../../include/abstract/Collider.h"
#include "Registry.h"

void PhysicsSystem::update(float dt)
{
    registry.each<Collider>([this](GameObject go, Collider&)
    {
        if (registry.has<Transform>(go) && !world->hasObject(go))
            world->addObject(go);
    });

    accumulator += std::min(dt, MAX_ACCUMULATED);

    while (accumulator >= FIXED_DT)
    {
        world->step(FIXED_DT);
        accumulator -= FIXED_DT;
    }

    world->syncTransforms();
}
