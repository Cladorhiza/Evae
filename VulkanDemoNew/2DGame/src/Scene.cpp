#include "Scene.h"

Scene::Scene()
	:entities{}, nextID(0)
{




}

Entity& Scene::GetEntity(uint32_t id) {
	if (entities.find(id) != entities.end()) {
		return entities.at(id);
	}
	std::cout << "Attempted to retrieve entity " << id << " from scene, DOESN'T EXIST.\n";
	Entity e{ 666666 };
	return e;
}

uint32_t Scene::AddEntity() {
	
	if (entities.find(nextID) == entities.end()) {
		entities[nextID] = Entity{ nextID };
	}
	nextID++;

	return nextID - 1;

}

void Scene::MainLoop() {

    //delta-time storage
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::duration<float> elapsed;

    //used to test how many frames per second
    double d = 0.0;
    int framesCount = 0;

    //main game loop
    while (!glfwWindowShouldClose(window)) {

        start = std::chrono::high_resolution_clock::now();

        //poll inputs
        InputManager::Poll(window);

        //update logic and draw
        Camera::Update(elapsed.count());

        updateUniformBuffer(currentFrame, d);

        drawFrame();


        end = std::chrono::high_resolution_clock::now();

        //calc delta time
        elapsed = (end - start);

        //framerate calculation
        d += elapsed.count();
        framesCount++;
        if (d > std::chrono::seconds(1).count()) {
            d -= std::chrono::seconds(1).count();
            std::cout << framesCount << std::endl;
            framesCount = 0;
        }

        //exit program on escape
        if (InputManager::GetKeyState(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            break;
        }
    }

    vkDeviceWaitIdle(device->GetDevice());
}