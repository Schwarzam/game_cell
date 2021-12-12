//
// Created by gusta on 20/11/2021.
//

#include "GameManager.h"
#include <utility>

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

GameManager::GameManager(sf::RenderWindow *window) :
        _window(window),
        fog(candle::LightingArea::FOG, sf::Vector2f(0.f, 0.f), sf::Vector2f(1600.f, 1600.f)){
    light.setRange(350);
    light.setFade(true);

    fog.setAreaColor(sf::Color::Black);
}



void GameManager::startGame(const std::string& mapname) {
    //servers = ServerMenu();
    //servers.RefreshInternetServers();

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
        if (!entity.second->isMyPlayer()){
            _window->draw(*entity.second);
        }
    }
    for (auto& entity : entities){
        if (entity.second->isMyPlayer()){

            light.setPosition(entity.second->getPosition());

            //candle::EdgeVector edges = Map::getEdges();
            //light.castLight(edges.begin(), edges.end());

            _window->draw(*entity.second);

            fog.clear();
            fog.draw(light);
            fog.display();

            _window->draw(fog);
        }
    }

}


