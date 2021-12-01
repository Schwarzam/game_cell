//
// Created by gusta on 01/12/2021.
//

#include "BaseMenu.h"


BaseMenu::BaseMenu(sf::RenderWindow *window) : _window(window) {

}


void BaseMenu::clear() {
    MenuButtons.clear();
}

void BaseMenu::addButton(const std::string& buttonName, State action) {
    MenuButtons.emplace_back(buttonName, action);
}

void BaseMenu::processMenuUpdates() {

}

void BaseMenu::render() {
    int height = 0, space = 0;

    for (auto& item : MenuButtons){

    };
}



