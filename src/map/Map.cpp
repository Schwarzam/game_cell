//
// Created by gusta on 22/11/2021.
//

#include "Map.h"
#include "../Assets.h"

Map::Map(std::string name) {
    _ptexture = Assets::Acquire("maps/teste");
    _sprite.setTexture(*_ptexture);
}

void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    //Draw base layer
    target.draw(_sprite, states);
}
