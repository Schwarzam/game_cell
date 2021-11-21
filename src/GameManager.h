//
// Created by gusta on 20/11/2021.
//

#ifndef THECELL_GAMEMANAGER_H
#define THECELL_GAMEMANAGER_H


#include <vector>
#include <SFML/System/Thread.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Entity.h"

class GameManager {
public:
    GameManager() = default;

    static Entity *startMyPlayer(const std::string& name, sf::RenderWindow *window);
    static Entity *startEntity(const std::string& name);

//    static void sortRender();
    static void gameEvents();
//
//    static bool validatePos(Entity *ent, sf::Sprite& _sprite, const sf::Vector2f &movement);
private:

protected:
    static std::vector<std::pair<float, Entity*>> entities;
};



#endif //THECELL_GAMEMANAGER_H
