//
// Created by gusta on 01/12/2021.
//

#ifndef THECELL_BASEMENU_H
#define THECELL_BASEMENU_H


#include <vector>
#include <string>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../State.h"

class BaseMenu {
public:
    BaseMenu(sf::RenderWindow *window);

    void clear();
    void addButton(const std::string& buttonName, State action);

    void processMenuUpdates();
    void render();
private:
    std::vector<std::pair<std::string, int>> MenuButtons;
    sf::RenderWindow *_window;
};


#endif //THECELL_BASEMENU_H
