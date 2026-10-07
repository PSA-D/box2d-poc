#pragma once

#include "abstract/Component.h"

enum class BodyType { Static, Kinematic, Dynamic };

class Rigidbody : public Component {
    public:
        Rigidbody() = default;

        BodyType getBodyType() const { return bodyType; }
        void setBodyType(const BodyType& newType) { bodyType = newType; }

        bool isFixedRotation() const { return fixedRotation; }
        void setFixedRotation(const bool newFixedRotation) { fixedRotation = newFixedRotation; }

        float getMass() const { return mass; }
        void setMass(const float newMass) { mass = newMass; }

        float getGravityScale() const { return gravityScale; }
        void setGravityScale(const float newGravityScale) { gravityScale = newGravityScale; }

    private:
        BodyType bodyType {BodyType::Dynamic};
        float mass {1.0f};
        float gravityScale {1.0f};
        bool fixedRotation {false};
};