//
// Created by Hristo Dinkov on 3.11.2025 г..
//

#include "SceneManager.h"
#include "Random.h"
#include <iostream>

#include "SinMovement.h"

SceneManager::SceneManager(core::Model cube) : model(cube) {}

std::shared_ptr<Scene> SceneManager::createScene(const std::string& name) {
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    scenesDictionary[name] = scene;
    if (!currentScene) {
        currentScene = scene;
    }
    return scene;
}

void SceneManager::setActiveScene(const std::string& name) {
    auto sceneToActivate = scenesDictionary.find(name); //scenes.end() is a special iterator that points one past the last valid element in the map
    if (sceneToActivate != scenesDictionary.end()) {
        currentScene = sceneToActivate->second;
        //std::cout << "Switched to scene: " << name << std::endl;
    }

}

std::shared_ptr<Scene> SceneManager::getActiveScene() const {
    return currentScene;
}

void SceneManager::update(float deltaTime) {
    if (currentScene)
        currentScene->update(deltaTime);
}

void SceneManager::render(Shader& shader,const glm::mat4& projection,const glm::mat4& view)
{
    if (!currentScene)
        return;

    currentScene->render(shader, projection, view);
}

int SceneManager::getSatCount() const {
    return currentScene->getSatTestsThisFrame();
}

double SceneManager::getSatTime() const {
    return currentScene->getSatTimeThisFrame();
}
void SceneManager::spawnCubesInScene(int count,int seed) {
    clearObjects();
    auto scene = getActiveScene();
    if (!scene) return;


    //float border = cbrtf((float)count) * 0.5;

    for (int i = 0; i < count; i++) {
        auto cube = scene->addObject(std::make_shared<GameObject>("Cube"+std::to_string(i)));

        glm::vec3 r = rand3(glm::vec3(i,seed,i*0.5),seed);

        float x = (r.x*2.0f-1.0f)*count;
        float y = (r.y*2.0f-1.0f)*count;
        float z = (r.z*2.0f-1.0f)*count;

        cube->setPos(glm::vec3(x,y,z));

        cube->model = model;
        cube->collider = std::make_shared<ConvexCollider>(cube->model->getAllVertices(),cube->model->getAllIndices(),cube->getWorldTransform());
        cube->addBehavior(std::make_shared<SinMovement>(floor(r.x*3),0.5f,0.5f,r.y*6.28,count));
    }
}
void SceneManager::clearObjects() {
    getActiveScene()->objects.clear();
}


