//
// Created by gusta on 20/11/2021.
//

#ifndef THECELL_CORE_H
#define THECELL_CORE_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>
#include "GameManager.h"
#include "utils/fps.h"
#include "map/Map.h"
#include "utils/BaseMenu.h"
#include "State.h"
#include "old/DedicatedServer.h"
#include "P2Pserver.h"
#include "P2PClient.h"

#include <SFML/System.hpp>

class Core  {
public:
    Core(); //< constructor
    Core(const Core&) = delete;
    Core& operator=(const Core&) = delete;

    void runWithMinimumTimeSteps(int minimum_frame_per_seconds=30);

public:

    State gameState;


private:
    P2Pserver *server;
    sf::Thread thread;

    State lastGameState;
    GameManager *game;

    P2PClient client;

    void processEvents();//< Process events
    void processCoreState();

    void update(); //< do some updates
    void render();//< draw all the stuff

    sf::RenderWindow _window; //< the window use to display the game
    BaseMenu menu;

    FPS fps;
};


#endif //THECELL_CORE_H
