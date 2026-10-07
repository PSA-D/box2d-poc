#pragma once

class Component
{
    public:
        virtual ~Component() = default;

        virtual bool isActive() const { return active; }
        virtual void setActive(const bool newValue) { active = newValue; }
    protected:
        bool active {false};
};