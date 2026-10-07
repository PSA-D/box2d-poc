#include "BoxColliderBox2D.h"

bool BoxColliderBox2D::collidesWith(const Collider& other) const
{
    return isTouching(other);
}

void BoxColliderBox2D::attachTo(b2BodyId body)
{
    const b2ShapeDef def = makeShapeDef(*this);

    // Box2D takes HALF extents; our Vec2 size is the full width/height.
    const b2Polygon box = b2MakeOffsetBox(size.x * 0.5f, size.y * 0.5f,
                                          b2Vec2{offset.x, offset.y}, b2Rot_identity);
    shapeId = b2CreatePolygonShape(body, &def, &box);
}
