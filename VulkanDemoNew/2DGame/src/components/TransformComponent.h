#pragma once
#include "../Component.h"
#include "glm.hpp"
#include "gtx/transform.hpp"
class TransformComponent :
    protected Component
{
protected:

    glm::mat4 transform;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

public:

    TransformComponent();
    inline glm::mat4 GetTransform(){ return transform; }
    inline glm::vec3 GetPosition() { return position; }
    inline glm::vec3 GetRotation() { return rotation; }
    inline glm::vec3 GetScale() { return scale; }
    void SetPosition(const glm::vec3& newPosition);
    void SetRotation(const glm::vec3& newRotation);
    void SetScale(const glm::vec3& newScale);
    void UpdateTransform();

};

