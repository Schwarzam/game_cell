//
// Created by gusta on 20/11/2021.
//

#ifndef THECELL_FPS_H
#define THECELL_FPS_H


#include <SFML/System/Clock.hpp>
#include <string>
#include <sstream>

class FPS
{
public:
    /// @brief Constructor with initialization.
    ///
    FPS(sf::RenderWindow *window) : mFrame(0), mFps(0), _window(window) {
        font.loadFromFile("assets/fonts/Bungee-Regular.ttf");
    }

    /// @brief Update the frame count.
    ///


    /// @brief Get the current FPS count.
    /// @return FPS count.

    std::string getFPS() {
        update();
        std::ostringstream ss;
        ss << mFps;
        return ss.str();
    }

    void drawFPS() {
        update();
        std::ostringstream ss;
        ss << mFps;

        sf::Text textName;
        textName.setFont(font);
        textName.setString(ss.str());
        textName.setPosition(32 + _window->getView().getCenter().x - _window->getView().getSize().x/2,
                             (float)_window->getPosition().y + (float)_window->getSize().y - 32  + _window->getView().getCenter().y- _window->getView().getSize().y/2);
        textName.setCharacterSize(16);
        _window->draw(textName);
    }



private:
    sf::Font font;
    sf::RenderWindow *_window;
    unsigned int mFrame;
    unsigned int mFps;
    sf::Clock mClock;

public:
    void update()
    {
        if(mClock.getElapsedTime().asSeconds() >= 1.f)
        {
            mFps = mFrame;
            mFrame = 0;
            mClock.restart();
        }

        ++mFrame;
    }
};


#endif //THECELL_FPS_H
