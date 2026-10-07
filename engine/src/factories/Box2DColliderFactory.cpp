#include "Box2DColliderFactory.h"

#include "../components/BoxColliderBox2D.h"
#include "../components/CircleColliderBox2D.h"

std::unique_ptr<BoxCollider> Box2DColliderFactory::createBoxCollider(Vec2 size)
{
    return std::make_unique<BoxColliderBox2D>(size);
}

std::unique_ptr<CircleCollider> Box2DColliderFactory::createCircleCollider(float radius)
{
    return std::make_unique<CircleColliderBox2D>(radius);
}
