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
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include "../State.h"

struct Button{
    std::string name;
    State buttonOperation{};
    std::shared_ptr<sf::Texture> _ptexture;
    sf::Sprite _sprite;
};

class BaseMenu {
public:
    BaseMenu(sf::RenderWindow *window, State *gameState);

    void clear();
    void addButton(const std::string& buttonName, State action);
    void createMainMenu();
    void createPauseMenu();

    void processMenuUpdates();
    void render();

    void openClose();
    const bool &isOpened() const;


private:
    State *gameState;
    sf::Font font;

    std::vector<Button> MenuButtons;
    sf::RenderWindow *_window;

    bool checkIfButtonCanBePressed();
    sf::Clock selectionClock;
    float totalDelta;

    int selected = 0, buttonsCount = 0;
    std::shared_ptr<sf::Texture> _ptexture;
    sf::Sprite _spriteSelected;

    bool isopen = false;

    std::shared_ptr<sf::Texture> _ptextureBackground;
    sf::Sprite _spriteBackground;
};


#endif //THECELL_BASEMENU_H
