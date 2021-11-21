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

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    std::shared_ptr<sf::Texture> _ptexture;

protected:
    sf::Vector2f movement;
    float velocity = 5;

    sf::Sprite _sprite;
};


#endif //THECELL_ENTITY_H
