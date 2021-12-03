//
// Created by gusta on 20/11/2021.
//

#include "GameManager.h"
#include "GameManager.h"

#include <cmath>
#include <iostream>
#include <utility>
#include "Collision.h"
#include "Player.h"


std::vector<std::pair<float, Entity*>> GameManager::entities;

Entity *GameManager::startEntity(const std::string& name) {
    auto *ent = new Entity(name);
    entities.emplace_back(std::pair(0.0, ent));
    return ent;
}

Entity *GameManager::startMyPlayer(const std::string &name, sf::RenderWindow *window, sf::Vector2f pos) {
    auto *ent = new Player(name, window, pos);
    entities.emplace_back(std::pair(0.0, ent));
    return ent;
}

GameManager::GameManager(sf::RenderWindow *window) : _window(window) {


}



void GameManager::startGame(const std::string& mapname) {
    isRunning = true;

    delete map;
    map = new Map(mapname);

    //First start my own player
    Entity *p1 = startMyPlayer("cleiton", _window, sf::Vector2f(300, 300));
}

void GameManager::endGame() {
    isRunning = false;
    entities.clear();
}

void GameManager::processEvents() {
    for (auto& entity : entities){
        entity.second->processEvents();
    }
}

bool GameManager::running() const {
    return isRunning;
}

void GameManager::render() {
    _window->draw(*map);

    for (auto& entity : entities){
        _window->draw(*entity.second);
    }
}


