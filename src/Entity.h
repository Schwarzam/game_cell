//
// Created by gusta on 20/11/2021.
//

#ifndef THECELL_ENTITY_H
#define THECELL_ENTITY_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Clock.hpp>
#include "Assets.h"


class Entity : public sf::Drawable, public sf::Transform {
public:
    explicit Entity(const std::string& name);
    sf::Vector2f getPosition();

    virtual void processEvents();
    void setPosition(sf::Vector2f pos);
    void setPosition(float x, float y);
    virtual void move(sf::Vector2f tomove);

    const sf::Sprite& getSprite() const;
    const sf::Rect<float>& getRect() const;
    const sf::Vector2f& getSpriteSize() const;

    const bool &isMyPlayer() const;
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    std::shared_ptr<sf::Texture> _ptexture;

protected:
    sf::Vector2f movement;
    float velocity = 300;

    sf::Sprite _sprite;
    sf::Vector2f _spriteSize;
    sf::Rect<float> playerRect;

    bool myPlayer = false;
};


#endif //THECELL_ENTITY_H
