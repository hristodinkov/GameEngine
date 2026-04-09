//
// Created by Hristo Dinkov on 23.10.2025 г..
//

#include "Scene.h"
#include "../MyClasses/CollisionSystem/Collision.h"

std::shared_ptr<GameObject> Scene::addObject(std::shared_ptr<GameObject> obj) {
    objects.push_back(obj);
    return obj;
}

void Scene::update(float deltaTime) {
    satTestsThisFrame = 0;
    satTimeThisFrame = 0;
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->isColliding = false;
        objects[i]->update(deltaTime);
    }

    for (int i = 0; i < objects.size(); i++) {
        for (int j = i + 1; j < objects.size(); j++) {

            auto& A = *objects[i];
            auto& B = *objects[j];

            A.collider->update(A.getWorldTransform());
            B.collider->update(B.getWorldTransform());

            satTestsThisFrame++;

            auto start = std::chrono::high_resolution_clock::now();

            Collision col;
            bool hit = col.SATCollision(*A.collider, *B.collider);

            auto end = std::chrono::high_resolution_clock::now();
            double seconds = std::chrono::duration<double>(end - start).count();
            satTimeThisFrame += seconds;

            if (hit) {
                A.isColliding = true;
                B.isColliding = true;
            }
        }
    }

}

void Scene::render(Shader& shader,const glm::mat4& projection,const glm::mat4& view)
{
    for (auto& obj : objects)
        obj->render(shader, projection, view);
}
int Scene::getSatTestsThisFrame() {
    return satTestsThisFrame;
}
double Scene::getSatTimeThisFrame() {
    return satTimeThisFrame;
}






