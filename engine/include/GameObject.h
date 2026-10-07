#pragma once

class GameObject
{
public:
    GameObject();
    explicit GameObject(int existingId) : id(existingId) {}
    int getId() const { return id; }

private:
    int id;
};
