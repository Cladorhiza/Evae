#include "Scene.h"
#include "Renderer.h"

#include <iostream>

int main()
{
    Scene scene;
    uint32_t entityIds[10]{ 0 };
    for (int i{ 0 }; i < 10; i++) {
        entityIds[i] = scene.AddEntity();
    }
    scene.GetEntity(5).SetHasTransform(true);

    Renderer renderer;

    renderer.Init();

    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");
    renderer.AddSprite("res/textures/bruno.png");

    renderer.Render();
    
}