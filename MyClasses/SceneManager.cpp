//
// Created by Hristo Dinkov on 3.11.2025 г..
//

#include "SceneManager.h"
#include "Random.h"
#include <iostream>

#include "Rotate.h"
#include "SinMovement.h"
#include "CollisionSystem/Collision.h"


std::shared_ptr<Scene> SceneManager::createScene(const std::string& name) {
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    scenesDictionary[name] = scene;
    if (!currentScene) {
        currentScene = scene;
    }
    return scene;
}

void SceneManager::setActiveScene(const std::string& name) {
    auto sceneToActivate = scenesDictionary.find(name);
    if (sceneToActivate != scenesDictionary.end()) {
        currentScene = sceneToActivate->second;
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
    return satTestsThisFrame;
}

double SceneManager::getSatTime() const {
    return satTimeThisFrame;
}
void SceneManager::resetSatStats() {
    satTestsThisFrame = 0;
    satTimeThisFrame = 0.0f;
}

void SceneManager::spawnCubesInScene(int count,int seed, const core::Model& model) {
    clearObjects();
    auto scene = getActiveScene();
    if (!scene) return;


    float border = count*0.3;

    for (int i = 0; i < count; i++) {
        auto cube = scene->addObject(std::make_shared<GameObject>("Cube"+std::to_string(i)));

        glm::vec3 r = rand3(glm::vec3(i,seed,i*0.5),seed);

        float x = (r.x*2.0f-1.0f)*border;
        float y = (r.y*2.0f-1.0f)*border;
        float z = (r.z*2.0f-1.0f)*border;

        cube->setPos(glm::vec3(x,y,z));

        float amplitude = border ;
        cube->model = model;
        cube->collider = std::make_shared<ConvexCollider>(cube->model->getAllVertices(),cube->model->getAllIndices(),cube->getWorldTransform());
        cube->addBehavior(std::make_shared<SinMovement>(floor(r.x*3),0.5f,amplitude,r.y*6.7f,border));

    }
}
void SceneManager::clearObjects() {
    getActiveScene()->objects.clear();
}

std::vector<std::pair<GameObject *, GameObject *> > SceneManager::computeBruteForcePairs() {
    std::vector<std::pair<GameObject*, GameObject*>> pairs;

    auto& objs = currentScene->objects;

    for (size_t i = 0; i < objs.size(); i++) {
        for (size_t j = i + 1; j < objs.size(); j++) {
            pairs.emplace_back(objs[i].get(), objs[j].get());
        }
    }

    return pairs;
}

void SceneManager::runSAT(GameObject *A, GameObject *B) {
    auto start = std::chrono::high_resolution_clock::now();

    Collision col;
    bool hit = col.SATCollision(*A->collider, *B->collider);

    auto end = std::chrono::high_resolution_clock::now();
    double deltaTime = std::chrono::duration<double>(end - start).count();

    satTestsThisFrame++;
    satTimeThisFrame += deltaTime;

    if (hit) {
        A->isColliding = true;
        B->isColliding = true;
    }
}






