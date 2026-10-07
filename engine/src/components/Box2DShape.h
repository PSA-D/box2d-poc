#pragma once

#include <box2d/box2d.h>

class Collider;

// Implemented by every Box2D collider. Lets Box2DPhysicsWorld attach a collider to a
// body without knowing whether it is a box or a circle.
class Box2DShape
{
    public:
        virtual ~Box2DShape() = default;

        // Creates the b2Shape on `body`, using the collider's density/friction/etc.
        virtual void attachTo(b2BodyId body) = 0;

        // Forget the shape (the body that owned it is gone).
        void detach() { shapeId = b2_nullShapeId; }

    protected:
        static b2ShapeDef makeShapeDef(const Collider& collider);

        // True if this shape is currently touching `other` in the Box2D world.
        bool isTouching(const Collider& other) const;

        b2ShapeId shapeId = b2_nullShapeId;
};
