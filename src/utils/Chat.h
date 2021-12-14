//
// Created by gusta on 14/12/2021.
//

#ifndef THECELL_CHAT_H
#define THECELL_CHAT_H


#include <SFML/Graphics/Drawable.hpp>
#include <string>
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>

class Chat{
public:
    Chat(sf::RenderWindow *window);
    ~Chat();

    static void addMessage(std::string message);

    void drawChat();
private:
    static std::vector<std::string> messages;

    sf::RenderWindow *_window;
    sf::Font font;
};


#endif //THECELL_CHAT_H
