//
// Created by gusta on 21/11/2021.
//

#ifndef THECELL_PLAYER_H
#define THECELL_PLAYER_H


#include <SFML/Graphics/RenderWindow.hpp>
#include "Entity.h"

class Player : public Entity {
public:
    explicit Player(const std::string& name, sf::RenderWindow *window);

    void processEvents() override;
    void ProcessKeyboardInputs();
    void ProcessMouseInputs();

private:
    sf::Clock MovementClock;
    sf::RenderWindow *_window;

    sf::View view;
};


#endif //THECELL_PLAYER_H
