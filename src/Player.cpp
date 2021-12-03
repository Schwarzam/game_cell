//
// Created by gusta on 21/11/2021.
//

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include "Player.h"
#include <cmath>
#include <iostream>
#include "map/Map.h"

Player::Player(const std::string &name, sf::RenderWindow *window, sf::Vector2f position) : Entity(name), _window(window) {
    view = window->getView();
    setPosition(position);
}

void Player::ProcessKeyboardInputs() {
    movement = sf::Vector2f(0, 0);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        movement.y -= velocity;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        movement.y += velocity;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        movement.x -= velocity;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        movement.x += velocity;
    }

    float deltaTime = MovementClock.restart().asSeconds();
    movement = movement * deltaTime;

    if (movement.x != 0 && movement.y != 0){
        movement.x = (float) (movement.x / sqrt(2));
        movement.y = (float) (movement.y / sqrt(2));
    }
    Map::move(this, movement);
}

void Player::processEvents() {
    ProcessKeyboardInputs();
    ProcessMouseInputs();

    view.setCenter(getPosition().x, getPosition().y);
    _window->setView(view);
}

void Player::ProcessMouseInputs() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {

    }
    sf::Vector2<int> mousePosition = sf::Mouse::getPosition(*_window);

    sf::Vector2f worldPos = _window->mapPixelToCoords(mousePosition);

    const float PI = 3.14159265;
    float rotation = (std::atan2((float)worldPos.y - getPosition().y,
                                 (float)worldPos.x - getPosition().x)) * 180 / PI;

    _sprite.setRotation(rotation + 90);
}




