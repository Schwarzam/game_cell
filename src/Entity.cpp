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

    _spriteSize.x = (float)_sprite.getTexture()->getSize().x * _sprite.getScale().x;
    _spriteSize.y = (float)_sprite.getTexture()->getSize().y * _sprite.getScale().y;

    playerRect.width = _spriteSize.x;
    playerRect.height = _spriteSize.y;
}

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(_sprite, states);
}

void Entity::processEvents() {
    _sprite.move(0.01, 0.01);
}

void Entity::setPosition(sf::Vector2f pos) {
    _sprite.setPosition(pos);

    playerRect.left = _sprite.getPosition().x + _spriteSize.x/2;
    playerRect.top = _sprite.getPosition().y + _spriteSize.y/2;
}

void Entity::setPosition(float x, float y) {
    _sprite.setPosition(x, y);

    playerRect.left = _sprite.getPosition().x + _spriteSize.x/2;
    playerRect.top = _sprite.getPosition().y + _spriteSize.y/2;
}

void Entity::move(sf::Vector2f tomove) {
    _sprite.setPosition(_sprite.getPosition().x + tomove.x, _sprite.getPosition().y + tomove.y);
    playerRect.left = _sprite.getPosition().x - _spriteSize.x/2;
    playerRect.top = _sprite.getPosition().y - _spriteSize.y/2;
}

const sf::Sprite &Entity::getSprite() const {
    return _sprite;
}

const sf::Rect<float> &Entity::getRect() const{
    return playerRect;
}

const sf::Vector2f &Entity::getSpriteSize() const{
    return _spriteSize;
}


