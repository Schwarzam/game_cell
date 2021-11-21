//
// Created by gusta on 20/11/2021.
//

#include "Game.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>

#include "../steam/steam_api.h"

Game::Game() : _window(sf::VideoMode(1200, 720),"Game hld", sf::Style::Resize)
{
    view.setSize((float)_window.getSize().x, (float)_window.getSize().y);

    Entity *p1 = startMyPlayer("cleiton", &_window);
    p1->setPosition(sf::Vector2f(300, 300));
}

void Game::runWithMinimumTimeSteps(int minimum_frame_per_seconds) {
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

void Game::update()
{
    for (auto& entity : entities){
        entity.second->processEvents();
    }
    //_player.processEvents();
    //_player.update(deltaTime);
}

void Game::render()
{
    //Clear screen
    _window.clear(sf::Color(155, 155, 155));

    for (auto& entity : entities){
        _window.draw(*entity.second);
    }

    //Count FPS
    _window.setTitle(fps.getFPS());

    //Update the window
    _window.display();
}

void Game::processEvents() {
    sf::Event event{};
    while(_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)//Close window
            _window.close();

        else if (event.type == sf::Event::KeyPressed) //keyboard input
        {
            if (event.key.code == sf::Keyboard::Escape)
                _window.close();
            if (event.key.code == sf::Keyboard::O){
                _window.setView(view);
            }
            if (event.key.code == sf::Keyboard::L) {
                _window.setView(view);
            }
        }

        else if (event.type == sf::Event::Resized) //keyboard input
        {
            _window.setView(view);
        }
    }
}

