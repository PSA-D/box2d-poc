#pragma once

#include "RigidBody.h"
#include "Types.h"
#include "abstract/Collider.h"

#include <memory>

class GameObject
{
    public:
        GameObject();

        int getId() const { return id; }

        // Transform. The physics world writes these every frame (see syncTransforms()).
        Vec2  position {0, 0};
        float rotation {0}; // radians

        void setRigidbody(std::unique_ptr<Rigidbody> rb) { rigidbody = std::move(rb); }
        Rigidbody* getRigidbody() const { return rigidbody.get(); }

        void setCollider(std::unique_ptr<Collider> c) { collider = std::move(c); }
        Collider* getCollider() const { return collider.get(); }

    private:
        int id;
        std::unique_ptr<Rigidbody> rigidbody;
        std::unique_ptr<Collider>  collider;
};
