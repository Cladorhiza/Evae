#include "Scene.h"
#include "Renderer.h"

#include <iostream>

int main()
{

    Renderer renderer;

    renderer.Init();

    for (int i{ 0 }; i< 50; i++)
        renderer.AddSprite("res/textures/grass.png");

    Scene scene;
    uint32_t entityIds[10]{ 0 };
    for (int i{ 0 }; i < 10; i++) {
        entityIds[i] = scene.AddEntity();
    }
    scene.GetEntity(5).SetHasTransform(true);
    
    
    
    
    
    renderer.Render();
    
}