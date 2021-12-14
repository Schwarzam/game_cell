//
// Created by gusta on 14/12/2021.
//

#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include "Chat.h"

std::vector<std::pair<std::string, float>> Chat::messages;
sf::Clock Chat::clock;

Chat::Chat(sf::RenderWindow *window) : _window(window)
{
    clock.restart();
    font.loadFromFile("assets/fonts/Bungee-Regular.ttf");
}

Chat::~Chat() = default;

void Chat::addMessage(const std::string& message) {
    messages.emplace_back(std::pair(message, clock.getElapsedTime().asSeconds()));
}

void Chat::drawChat() {
    int i = 0;
    for (auto & message : messages){
        if (message.second + 10 > clock.getElapsedTime().asSeconds()){
            sf::Text textName;
            textName.setFont(font);
            textName.setString(message.first);
            textName.setPosition(16 + _window->getView().getCenter().x - _window->getView().getSize().x/2,
                                 (float)_window->getPosition().y + (float)_window->getSize().y - (32.0f * ((float)i+1.0f))  + _window->getView().getCenter().y - _window->getView().getSize().y/2);
            textName.setCharacterSize(16);
            _window->draw(textName);
            i++;
        }else{
            messages.pop_back();
        }
    }
}


