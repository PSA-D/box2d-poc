#include "CircleColliderBox2D.h"

bool CircleColliderBox2D::collidesWith(const Collider& other) const
{
    return isTouching(other);
}

void CircleColliderBox2D::attachTo(b2BodyId body)
{
    const b2ShapeDef def = makeShapeDef(*this);
    const b2Circle circle{b2Vec2{offset.x, offset.y}, radius};
    shapeId = b2CreateCircleShape(body, &def, &circle);
}
