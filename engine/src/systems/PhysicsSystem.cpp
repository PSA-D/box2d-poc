#include "PhysicsSystem.h"

#include <algorithm>

void PhysicsSystem::update(float dt)
{
    accumulator += std::min(dt, MAX_ACCUMULATED);

    while (accumulator >= FIXED_DT)
    {
        world->step(FIXED_DT);
        accumulator -= FIXED_DT;
    }

    world->syncTransforms();
}
