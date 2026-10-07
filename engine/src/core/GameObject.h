#pragma once

class GameObject
{
    public:
        GameObject();
        int getId() const { return id; }

    private:
        int id;
};