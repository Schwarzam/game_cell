//
// Created by gusta on 14/12/2021.
//

#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include "Chat.h"

std::vector<std::string> Chat::messages;

Chat::Chat(sf::RenderWindow *window) : _window(window)
{
    font.loadFromFile("assets/fonts/Bungee-Regular.ttf");
}

Chat::~Chat() = default;

void Chat::addMessage(std::string message) {
    messages.emplace_back(message);
}

void Chat::drawChat() {
    for (int i = 0; i < 5; i++){
        if (i >= messages.size()) break;

        sf::Text textName;
        textName.setFont(font);
        textName.setString(messages[i]);
        textName.setPosition(16 + _window->getView().getCenter().x - _window->getView().getSize().x/2,
                             (float)_window->getPosition().y + (float)_window->getSize().y - (32.0f * ((float)i+1.0f))  + _window->getView().getCenter().y - _window->getView().getSize().y/2);
        textName.setCharacterSize(16);
        _window->draw(textName);
    }
}


