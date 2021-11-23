//
// Created by gusta on 21/11/2021.
//

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include "Player.h"
#include <cmath>
#include "map/Map.h"

Player::Player(const std::string &name, sf::RenderWindow *window) : Entity(name), _window(window) {
}

void Player::ProcessKeyboardInputs() {
    movement = sf::Vector2f(0, 0);
    velocity = 100;
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

    if (Map::validadePos(_sprite, movement)){
        _sprite.move(movement);
    }
}

void Player::processEvents() {
    ProcessKeyboardInputs();
    ProcessMouseInputs();
}

void Player::ProcessMouseInputs() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {

    }
    sf::Vector2<int> mousePosition = sf::Mouse::getPosition(*_window);
    getPosition();

    const float PI = 3.14159265;

    float rotation = (std::atan2((float)mousePosition.y - getPosition().y,
                                 (float)mousePosition.x - getPosition().x)) * 180 / PI;


    _sprite.setRotation(rotation + 90);
}




