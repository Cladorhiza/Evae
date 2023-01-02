#pragma once
#include "../Component.h"

#include <string>
class SpriteComponent :
    protected Component
{

protected:
    std::string spritePath;

public:

    SpriteComponent(std::string texPath);
};

