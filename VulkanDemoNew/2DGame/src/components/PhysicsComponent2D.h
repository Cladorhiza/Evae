#pragma once
#include "../Component.h"


#include "glm/glm.hpp"

class PhysicsComponent2D :
    protected Component
{

private:

    glm::vec3 velocity;
    
public:


    glm::vec3& GetVelocity() { return velocity; }
    void SetVelocity(glm::vec3 vel) { velocity = vel; }

};