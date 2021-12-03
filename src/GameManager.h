//
// Created by gusta on 20/11/2021.
//

#ifndef THECELL_GAMEMANAGER_H
#define THECELL_GAMEMANAGER_H


#include <vector>
#include <SFML/System/Thread.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Entity.h"
#include "map/Map.h"

class GameManager {
public:
    explicit GameManager(sf::RenderWindow *window);

    static Entity *startMyPlayer(const std::string& name, sf::RenderWindow *window, sf::Vector2f pos);
    static Entity *startEntity(const std::string& name);

    void startGame(const std::string& mapname);
    void endGame();

    bool running() const;
    void processEvents();

    void render();
private:
    sf::RenderWindow *_window;
    Map *map = nullptr;

    bool isRunning{};

protected:
    static std::vector<std::pair<float, Entity*>> entities;
};



#endif //THECELL_GAMEMANAGER_H
