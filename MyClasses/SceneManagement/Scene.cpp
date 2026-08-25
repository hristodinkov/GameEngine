//
// Created by Hristo Dinkov on 23.10.2025 г..
//

#include "../MyClasses/SceneManagement/Scene.h"
#include "../CollisionSystem/Collision.h"

std::shared_ptr<GameObject> Scene::addObject(std::shared_ptr<GameObject> obj) {
    objects.push_back(obj);
    return obj;
}

void Scene::update(float deltaTime) {
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->isColliding = false;
        objects[i]->update(deltaTime);
    }

}

const std::string& Scene::getName() const {
    return name;
}

void Scene::render(Shader& shader,const glm::mat4& projection,const glm::mat4& view)
{
    for (auto& obj : objects)
        obj->render(shader, projection, view);
}





