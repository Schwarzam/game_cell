//
// Created by gusta on 20/11/2021.
//

#ifndef THECELL_GAME_H
#define THECELL_GAME_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>
#include "GameManager.h"
#include "utils/fps.h"
#include "map/Map.h"

class Game : public GameManager {
public:
    Game(); //< constructor
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void runWithMinimumTimeSteps(int minimum_frame_per_seconds=30);

public:
    Map map;

private:
    void processEvents();//< Process events
    static void update(); //< do some updates
    void render();//< draw all the stuff

    sf::RenderWindow _window; //< the window use to display the game


    FPS fps;
};


#endif //THECELL_GAME_H
