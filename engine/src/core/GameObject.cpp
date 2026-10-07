#include "../../include/GameObject.h"

namespace
{
    int nextId = 0;
}

GameObject::GameObject() : id(nextId++) {}
