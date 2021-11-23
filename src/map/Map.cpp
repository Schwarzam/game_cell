//
// Created by gusta on 22/11/2021.
//

#include <fstream>
#include <iostream>
#include "Map.h"
#include "../Assets.h"

nlohmann::json Map::map_metadata;
std::vector<sf::Rect<float>> Map::walls;

Map::Map(std::string folder) {
    mapfolder = "maps/" + folder + "/";
    _ptexture = Assets::Acquire(mapfolder + "static");
    _sprite.setTexture(*_ptexture);


    std::ifstream file("assets/" + mapfolder + "metadata.json");
    file >> map_metadata;

    sf::Vector2i utilSize = sf::Vector2i(map_metadata["tilewidth"].get<int>(), map_metadata["tileheight"].get<int>());
    sf::Vector2i mapHeight = sf::Vector2i(map_metadata["height"].get<int>(), map_metadata["width"].get<int>());

    for (int i = 0; i < map_metadata["layers"].size(); i++){
        if (map_metadata["layers"][i]["name"] == "walls"){
            for (int dr = 0; dr < map_metadata["layers"][i]["data"].size(); dr++){
                if (map_metadata["layers"][i]["data"][dr].get<int>() != 0){
                    std::cout << (dr / mapHeight.x) * utilSize.x << " " << (dr % mapHeight.y) * utilSize.y << std::endl;
                    walls.emplace_back(sf::Rect<float>((dr % mapHeight.y) * utilSize.y, (dr / mapHeight.x) * utilSize.x, utilSize.x, utilSize.y));
                }
            }
        }
    }
}

void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    //Draw base layer
    target.draw(_sprite, states);
}

bool Map::validadePos(sf::Sprite &sprite, sf::Vector2f movement) {
    sf::Sprite spriteCopy = sprite;
    spriteCopy.move(movement);

    for (auto & rect : walls){
        if (rect.intersects(sprite.getGlobalBounds())){
            if (sprite.getGlobalBounds().left < rect.left
            && sprite.getGlobalBounds().left + sprite.getGlobalBounds().width < rect.left + rect.width
            && sprite.getGlobalBounds().top < rect.top + rect.height
            && sprite.getGlobalBounds().top + rect.height > rect.top
            ){
                sprite.setPosition(rect.left - sprite.getGlobalBounds().width/2, sprite.getGlobalBounds().top + sprite.getGlobalBounds().height/2);
            }
            if (sprite.getGlobalBounds().left > rect.left
            && sprite.getGlobalBounds().left + sprite.getGlobalBounds().width > rect.left + rect.width
            && sprite.getGlobalBounds().top < rect.top + rect.height
            && sprite.getGlobalBounds().top + rect.height > rect.top
            ){
                sprite.setPosition(rect.left + rect.width + sprite.getGlobalBounds().width/2, sprite.getGlobalBounds().top + sprite.getGlobalBounds().height/2);
            }

            return false;
        }


    }
    return true;
}
