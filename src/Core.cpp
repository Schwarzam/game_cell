//
// Created by gusta on 20/11/2021.
//

#include "Core.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include <SFML/Graphics/CircleShape.hpp>

#include "../steam/steam_api.h"
#include "old/DedicatedServer.h"
#include "utils/ServerMenu.h"

void launchServer(){

}

Core::Core() : _window(sf::VideoMode(1200, 720, 32),"The Cell", sf::Style::Fullscreen),
                menu(&_window, &gameState), game(new GameManager(&_window)), thread(&launchServer), fps(&_window)
{
    menu.openClose();
    menu.createMainMenu();

    gameState = Menu;

    server = new P2Pserver();
    thread.launch();

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();


    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    _window.create(modes[0], "The Cell", sf::Style::Fullscreen);
//    for (std::size_t i = 0; i < modes.size(); ++i)
//    {
//        sf::VideoMode mode = modes[i];
//        std::cout << "Mode #" << i << ": "
//                  << mode.width << "x" << mode.height << " - "
//                  << mode.bitsPerPixel << " bpp" << std::endl;
//
//        _window.create(mode, "The Cell", sf::Style::Fullscreen);
//        break;
//    }
    _window.setVerticalSyncEnabled(true);
}

void Core::runWithMinimumTimeSteps(int minimum_frame_per_seconds) {
    sf::Clock clock;

    sf::Time timeSinceLastUpdate;
    sf::Time TimePerFrame = sf::seconds(1.f/(float)minimum_frame_per_seconds);

    while (_window.isOpen())
    {
        processEvents();
        timeSinceLastUpdate = clock.restart();
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            update();
        }
        SteamAPI_RunCallbacks();

        update();
        render();
    }
}

void Core::update()
{
    if (game->running()){
        game->processEvents();
    }
    server->RunFrame();
    processCoreState();

    //if (server != nullptr){
    //    server->RunFrame();
    //}
}

void Core::render()
{
    //Clear screen
    _window.clear(sf::Color(59, 59, 59));

    if (menu.isOpened()){
        menu.render();
    }else{
        game->render();
    }

    //Count FPS
    fps.drawFPS();

    //Update the window
    _window.display();
}

void Core::processEvents() {
    sf::Event event{};

    if(menu.isOpened()){
        menu.processMenuUpdates();
    }

    while(_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)//Close window
            _window.close();

        else if (event.type == sf::Event::KeyPressed) //keyboard input
        {
            if (event.key.code == sf::Keyboard::Escape){
                menu.clear();
                menu.createPauseMenu();

                menu.openClose();
                gameState = PausingGame;
            }

            if (event.key.code == sf::Keyboard::P){
                //_window.setView(view);

            }
        }

        else if (event.type == sf::Event::Resized) //keyboard input
        {
            //_window.setView(view);
        }
    }
}

void Core::processCoreState() {

    if (gameState != lastGameState){
        if(gameState == StartGame){
            menu.openClose();
            game = new GameManager(&_window);
            game->startGame("d2");
        }

        if(gameState == ResumingGame){
            menu.openClose();
        }

        if(gameState == QuitGame){
            //delete server;
            _window.close();
        }

        if(gameState == Lobby){
            menu.createMainMenu();
            game->endGame();
            free(game);
        }
    }

    lastGameState = gameState;
}

