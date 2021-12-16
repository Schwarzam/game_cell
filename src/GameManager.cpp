//
// Created by gusta on 20/11/2021.
//

#include "GameManager.h"
#include <utility>
#include <functional>

std::vector<std::pair<float, Entity*>> GameManager::entities;
sf::Mutex *GameManager::mutex;

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
        fog(candle::LightingArea::FOG, sf::Vector2f(0.f, 0.f), sf::Vector2f(1600.f, 1600.f))
{
    mutex = new sf::Mutex();
    light->setRange(350);
    light->setFade(true);
    light->setColor(sf::Color::Yellow);

    fog.setAreaColor(sf::Color::Black);
}

void GameManager::startGame(const std::string& mapname) {
    //servers = ServerMenu();
    //servers.RefreshInternetServers();

    isRunning = true;

    free(map);
    map = new Map(mapname);

    //First start my own player
    Entity *p1 = startMyPlayer("cleiton", _window, sf::Vector2f(300, 300));

    thread = new sf::Thread(&GameManager::processLight, light);
    thread->launch();
}

void GameManager::endGame() {
    free(map);
    isRunning = false;
    entities.clear();

    if(thread != nullptr){
        thread->terminate();
    }
    if(thread) free(thread);
    if(mutex) free(mutex);
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
            light->setPosition(entity.second->getPosition());
            _window->draw(*entity.second);

            fog.clear();
            fog.draw(*light);
            fog.display();

            _window->draw(fog);
        }
    }

}

[[noreturn]] void GameManager::processLight(candle::RadialLight * plight) {
    sf::Clock procLights;
    unsigned int mFrame = 0;
    unsigned int mFps = 0;

    candle::EdgeVector edges = Map::getEdges();

    while(true){
        plight->castLight(edges.begin(), edges.end());
    }

}

sf::Mutex* GameManager::getMutex() {
    return mutex;
}


