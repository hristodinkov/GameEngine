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
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->isColliding = false;
        objects[i]->update(deltaTime);
    }

    for (int i = 0; i < objects.size(); i++) {
        for (int j = i + 1; j < objects.size(); j++) {

            auto& A = *objects[i];
            auto& B = *objects[j];

            // Update colliders first
            A.collider->update(A.getWorldTransform());
            B.collider->update(B.getWorldTransform());
            Collision col;
            if (col.SATCollision(*A.collider, *B.collider)) {
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


