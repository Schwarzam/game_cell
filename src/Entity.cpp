//
// Created by gusta on 20/11/2021.
//

#include "Entity.h"

sf::Vector2f Entity::getPosition() {
    return _sprite.getPosition();
}

Entity::Entity(const std::string& name) {
    _ptexture = Assets::Acquire(name);

    _sprite.setTexture(*_ptexture);
    _sprite.setOrigin((float)_ptexture->getSize().x/2, (float)_ptexture->getSize().y/2);

    _sprite.setScale(0.1, 0.1);
}

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(_sprite, states);
}

void Entity::processEvents() {
    _sprite.move(0.01, 0.01);
}

void Entity::setPosition(sf::Vector2f pos) {
    _sprite.setPosition(pos);
}

