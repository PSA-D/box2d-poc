#include "Box2DShape.h"

#include <vector>

#include "../../include/abstract/Collider.h"

b2ShapeDef Box2DShape::makeShapeDef(const Collider& collider)
{
    b2ShapeDef def = b2DefaultShapeDef();
    def.density              = collider.getDensity();
    def.material.friction    = collider.getFriction();
    def.material.restitution = collider.getRestitution();
    def.isSensor             = collider.isTrigger();
    return def;
}

bool Box2DShape::isTouching(const Collider& other) const
{
    const auto* otherShape = dynamic_cast<const Box2DShape*>(&other);
    if (otherShape == nullptr || B2_IS_NULL(shapeId) || B2_IS_NULL(otherShape->shapeId))
        return false;

    std::vector<b2ContactData> contacts(static_cast<size_t>(b2Shape_GetContactCapacity(shapeId)));
    const int count = b2Shape_GetContactData(shapeId, contacts.data(), static_cast<int>(contacts.size()));

    for (int i = 0; i < count; ++i)
    {
        const b2ContactData& c = contacts[static_cast<size_t>(i)];
        const bool involvesOther = B2_ID_EQUALS(c.shapeIdA, otherShape->shapeId) ||
                                   B2_ID_EQUALS(c.shapeIdB, otherShape->shapeId);
        if (involvesOther && c.manifold.pointCount > 0)
            return true;
    }
    return false;
}
