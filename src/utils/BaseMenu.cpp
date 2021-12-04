//
// Created by gusta on 01/12/2021.
//

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "BaseMenu.h"
#include "../Assets.h"
#include "../../steam/steam_api.h"


BaseMenu::BaseMenu(sf::RenderWindow *window, State *gameState) :_window(window), gameState(gameState), buttonsCount(0) {
    font.loadFromFile("assets/fonts/Bungee-Regular.ttf");

    _ptexture = Assets::Acquire("utils/menuSelected");
    _spriteSelected.setTexture(*_ptexture);
    _spriteSelected.setScale(sf::Vector2f(0.2f, 0.2f));

    _ptextureBackground = Assets::Acquire("utils/background");
    _spriteBackground.setTexture(*_ptextureBackground);
    _spriteBackground.setScale(sf::Vector2f(0.3f, 0.3f));
}

void BaseMenu::clear() {
    selected = 0, buttonsCount = 0;
    MenuButtons.clear();
}

void BaseMenu::addButton(const std::string& buttonName, State action) {
    Button b1;
    b1.name = buttonName;
    b1.buttonOperation = action;
    b1._ptexture = Assets::Acquire("utils/menuButton");
    b1._sprite.setTexture(*b1._ptexture);
    b1._sprite.setScale(sf::Vector2f(0.2f, 0.2f));

    MenuButtons.emplace_back(b1);
    buttonsCount++;
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
        if(checkIfButtonCanBePressed()){
            if (selected + 1 < buttonsCount){selected ++;}
        }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
        if(checkIfButtonCanBePressed()){
            if (selected - 1 >= 0){selected --;}
        }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)){
        if(checkIfButtonCanBePressed()){*gameState = MenuButtons[selected].buttonOperation;}
    }
}

void BaseMenu::render() {
    //Draw background
    _spriteBackground.setPosition(_window->getView().getCenter().x - _window->getView().getSize().x / 2 - 50,
                                  _window->getView().getCenter().y -_window->getView().getSize().y / 2 + 100);

    _window->draw(_spriteBackground);

    //Drawn name on screen
    sf::Text textName;
    textName.setFont(font);
    textName.setString(SteamFriends()->GetPersonaName());
    textName.setPosition(_window->getView().getCenter().x - _window->getView().getSize().x / 2 + 50,
                     _window->getView().getCenter().y -_window->getView().getSize().y / 2 + 50);
    textName.setCharacterSize(32);
    _window->draw(textName);

    //Draw each of the menu buttons
    int height = 100, count = 0;
    sf::Text text;
    for (auto& item : MenuButtons){
        sf::Vector2f position = sf::Vector2f(_window->getView().getCenter().x,
                                             _window->getView().getCenter().y -_window->getView().getSize().y / 2 + (float)height);
        text.setFont(font);
        text.setString(item.name);
        text.setCharacterSize(32);

        text.setPosition(position.x + 24, position.y + 10);
        text.setFillColor(sf::Color::White);

        if(selected == count){
            _spriteSelected.setPosition(position);
            _window->draw(_spriteSelected);
        }else{
            item._sprite.setPosition(position);
            _window->draw(item._sprite);
        }
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
    addButton("Resume", ResumingGame);
    addButton("Lobby", Lobby);
}






