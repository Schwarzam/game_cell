//
// Created by gusta on 01/12/2021.
//

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "BaseMenu.h"


BaseMenu::BaseMenu(sf::RenderWindow *window, State *gameState) : _window(window), gameState(gameState) {
    font.loadFromFile("assets/fonts/MPLUS1Code-Regular.ttf");
}

void BaseMenu::clear() {
    selected = 0;
    MenuButtons.clear();
}

void BaseMenu::addButton(const std::string& buttonName, State action) {
    MenuButtons.emplace_back(buttonName, action);
}

bool BaseMenu::checkIfButtonCanBePressed() {
    float delta = selectionClock.restart().asSeconds();
    totalDelta += delta;
    if (totalDelta > 0.25){
        totalDelta = 0;
        return true;
    }
    return false;
}

void BaseMenu::processMenuUpdates() {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
        if(checkIfButtonCanBePressed()){selected ++;}
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
        if(checkIfButtonCanBePressed()){selected --;}
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)){
        if(checkIfButtonCanBePressed()){
            *gameState = MenuButtons[selected].second;
        }
    }
}

void BaseMenu::render() {

    int height = 100, count = 0;
    for (auto& item : MenuButtons){

        sf::Text text;
        text.setFont(font);
        text.setString(item.first);
        text.setCharacterSize(32);
        if(selected == count){
            text.setFillColor(sf::Color::Red);
        }else{
            text.setFillColor(sf::Color::Black);
        }

        text.setPosition(_window->getView().getCenter().x,
                         _window->getView().getCenter().y -_window->getView().getSize().y / 2 + height);

        _window->draw(text);
        height += 100;
        count ++;
    };
}

void BaseMenu::openClose() {
    isopen = !isopen;
}

const bool &BaseMenu::isOpened() const {
    return isopen;
}

void BaseMenu::createMainMenu() {
    clear();
    addButton("Play", StartGame);
    addButton("Quit", QuitGame);
}

void BaseMenu::createPauseMenu() {
    clear();
    addButton("Resume", RunningGame);
    addButton("Return to Lobby", Lobby);
}






