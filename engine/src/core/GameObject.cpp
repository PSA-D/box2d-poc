#include "../../include/GameObject.h"

namespace
{
    int nextId = 0; // Hold next game object ID
}

GameObject::GameObject() : id(nextId++) {}
