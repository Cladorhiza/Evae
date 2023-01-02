#include "Hubbsi.h"
#include "components/PlayerControllerPaddleComponent.h"
#include "systems/PlayerControllerPaddleSystem.h"
#include "components/AIControllerPaddleComponent.h"
#include "systems/AIControllerPaddleSystem.h"

#include <iostream>

int main()
{

    Renderer renderer;
    InputManager inputManager;
    Scene scene;

    //load rendering framework
    renderer.Init();

    Window window{ renderer.GetWindow() };
    Camera camera(window);
    inputManager.Init(window);
    scene.Init(&renderer);

    PlayerControllerPaddleSystem paddleSystem;
    paddleSystem.Init(&inputManager, &scene);
    AIControllerPaddleSystem aiPaddleSystem;
    aiPaddleSystem.Init(&scene);
    
    uint32_t entityIds[10]{ 0 };
    for (int i{ 0 }; i < 10; i++) {
        entityIds[i] = scene.AddEntity();
        scene.GetEntity(i).SetHasTransform(true);
        scene.transformSystem.AddTransformComponent(i);
    }

    //pong ball
    scene.GetEntity(2).SetHasSprite(true);
    scene.spriteSystem.AddSpriteComponent(2, "res/textures/pongball.png");
    scene.transformSystem.GetTransformComponent(2).SetScale(glm::vec3(0.5f, 0.5f, 1.f));
    paddleSystem.AddComponent(2, GLFW_KEY_W, GLFW_KEY_S, 5.f, 4.25f, -4.25f);

    //background sprite
    scene.GetEntity(3).SetHasSprite(true);
    scene.spriteSystem.AddSpriteComponent(3, "res/textures/background.png");
    scene.transformSystem.GetTransformComponent(3).SetPosition(glm::vec3(0.f, 0.f, -1.f));
    scene.transformSystem.GetTransformComponent(3).SetScale(glm::vec3(16.f, 9.f, 1.f));

    //playerpaddle
    scene.GetEntity(4).SetHasSprite(true);
    scene.spriteSystem.AddSpriteComponent(4, "res/textures/brunowot.png");
    scene.transformSystem.GetTransformComponent(4).SetScale(glm::vec3(0.5f,2.5f,1.f));
    scene.transformSystem.GetTransformComponent(4).SetPosition(glm::vec3(-7.f, 0.f, 0.f));
    paddleSystem.AddComponent(4, GLFW_KEY_W, GLFW_KEY_S, 5.f, 3.25f, -3.25f);
    //enemypaddle
    scene.GetEntity(5).SetHasSprite(true);
    scene.spriteSystem.AddSpriteComponent(5, "res/textures/bruno.png");
    scene.transformSystem.GetTransformComponent(5).SetScale(glm::vec3(0.5f, 2.5f, 1.f));
    scene.transformSystem.GetTransformComponent(5).SetPosition(glm::vec3(7.f, 0.f, 0.f));
    aiPaddleSystem.AddComponent(5, 5.f, 3.25f, -3.25f);
    aiPaddleSystem.SetBallTransform(&scene.transformSystem.GetTransformComponent(2));




    for (int i{ 0 }; i < 10; i++) {
        std::cout << "Entity " << i << ((scene.GetEntity(i).HasTransform()) ? " Has a transform component.\n" : " Has not got a transform component.\n");
    }
    
    //delta-time storage
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::duration<float> elapsed;
    //used to test how many frames per second
    double cumulativeSecond = 0.0;
    float timeStep = 0.05f;
    int framesCount = 0;

    double degs = 0.0;

    bool isOrtho = true;

    //main game loop
    while (!window.WindowShouldClose() && (inputManager.GetKeyState(GLFW_KEY_ESCAPE) != GLFW_PRESS)) {

        start = std::chrono::high_resolution_clock::now();

        //poll inputs
        window.PollEvents();
        inputManager.Poll();

        if (inputManager.GetKeyToggle(GLFW_KEY_T)) {
            isOrtho = !isOrtho;
            camera.SetOrthographic(isOrtho);
        }
        
        //update game logic
        
        //if (degs > 360.0) {
        //    degs = 0.0 + (degs - 360.0);
        //}
        //degs += 10.0 * timeStep;
        //scene.transformSystem.GetTransformComponent(4).SetRotation(glm::vec3(0.f,0.f,0.f));
        //scene.transformSystem.GetTransformComponent(4).SetScale(glm::vec3(degs / 36.0,degs / 36.0,1.0));
        //scene.transformSystem.GetTransformComponent(4).SetPosition(glm::vec3(degs / 36.0,degs / 36.0,1.0));
        
        paddleSystem.Update(timeStep);
        aiPaddleSystem.Update(timeStep);

        camera.Update(timeStep, inputManager);
        
        scene.spriteSystem.Update();

        //draw
        renderer.SetViewMatrix(camera.GetViewingMatrix());
        glm::mat4 proj{ camera.GetProjectionMatrix() };
        renderer.SetProjMatrix(proj);
        renderer.Render();


        //calc delta time
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        timeStep = elapsed.count();

        //framerate calculation
        cumulativeSecond += elapsed.count();
        framesCount++;
        if (cumulativeSecond > std::chrono::seconds(1).count()) {
            cumulativeSecond -= std::chrono::seconds(1).count();
            std::cout << framesCount << std::endl;
            framesCount = 0;
        }

    }
    
    
}