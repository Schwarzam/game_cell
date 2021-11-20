//
// Created by gusta on 20/11/2021.
//

#include "GameManager.h"
#include "GameManager.h"

#include <cmath>
#include "Collision.h"


std::vector<std::pair<float, Entity*>> GameManager::entities;

Entity *GameManager::startEntity(const std::string& name) {
    auto *ent = new Entity(name);
    entities.emplace_back(std::pair(0.0, ent));
    return ent;
}

//void GameManager::sortRender() {
//    std::sort(entities.begin(), entities.end());
//    for (auto& i : entities){
//        i.first = i.second->getPosX();
//    }
//}