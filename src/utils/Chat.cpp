//
// Created by gusta on 14/12/2021.
//

#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include "Chat.h"

std::vector<std::tuple<std::string, float, sf::Color>> Chat::messages;
sf::Clock Chat::clock;

Chat::Chat(sf::RenderWindow *window) : _window(window)
{
    clock.restart();
    font.loadFromFile("assets/fonts/Bungee-Regular.ttf");
}

Chat::~Chat() = default;

void Chat::addMessage(const std::string& message, sf::Color color) {
    messages.emplace_back(std::tuple(message, clock.getElapsedTime().asSeconds(), color));
}

void Chat::drawChat() {
    int i = 0;
    for (auto & message : messages){
        if (std::get<1>(message) + 10 > clock.getElapsedTime().asSeconds()){
            sf::Text textName;
            textName.setFont(font);
            textName.setString(std::get<0>(message));
            textName.setPosition(16 + _window->getView().getCenter().x - _window->getView().getSize().x/2,
                                 (float)_window->getPosition().y + (float)_window->getSize().y - (32.0f * ((float)i+1.0f))  + _window->getView().getCenter().y - _window->getView().getSize().y/2);
            textName.setCharacterSize(16);
            textName.setFillColor(std::get<2>(message));
            _window->draw(textName);
            i++;
        }else{
            messages.pop_back();
        }
    }
}


