//
// Created by gusta on 22/11/2021.
//

#include <fstream>
#include <iostream>
#include "Map.h"
#include "../Assets.h"
#include "../Collision.h"

nlohmann::json Map::map_metadata;
std::vector<sf::Rect<float>> Map::walls;
float Map::mapH;
float Map::mapW;
float Map::mapBlockSize;

Map::Map(const std::string& folder) {
    mapfolder = "maps/" + folder + "/";
    _ptexture = Assets::Acquire(mapfolder + "static");
    _sprite.setTexture(*_ptexture);


    std::ifstream file("assets/" + mapfolder + "metadata.json");
    file >> map_metadata;

    sf::Vector2i utilSize = sf::Vector2i(map_metadata["tilewidth"].get<int>(), map_metadata["tileheight"].get<int>());
    sf::Vector2i mapHeight = sf::Vector2i(map_metadata["height"].get<int>(), map_metadata["width"].get<int>());

    mapH = map_metadata["height"].get<float>();
    mapW = map_metadata["width"].get<float>();
    mapBlockSize = map_metadata["tilewidth"].get<float>();

    for (int i = 0; i < map_metadata["layers"].size(); i++){
        if (map_metadata["layers"][i]["name"] == "walls"){
            for (int dr = 0; dr < map_metadata["layers"][i]["data"].size(); dr++){
                if (map_metadata["layers"][i]["data"][dr].get<int>() != 0){
                    walls.emplace_back(sf::Rect<float>((float)(dr % mapHeight.y) * utilSize.y, (dr / mapHeight.x) * utilSize.x, utilSize.x, utilSize.y));
                }
            }
        }
    }
}

void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    //Draw base layer
    target.draw(_sprite, states);
}

void Map::move(Player *player, sf::Vector2f movement) {
    sf::Sprite spriteCopy = player->getSprite();
    sf::Rect<float> playerBounds = player->getRect();
    sf::Rect<float> nextPos;


    for (auto & wallBounds : walls){
        nextPos = playerBounds;
        nextPos.left += movement.x;
        nextPos.top += movement.y;


        //Outside bottom map
        if (nextPos.top + nextPos.height > mapH * mapBlockSize){
            movement.y = 0.0f;
        }
        //Outside up map
        if (nextPos.top < 0){
            movement.y = 0.0f;
        }
        //Outside right map
        if (nextPos.left + nextPos.width > mapW * mapBlockSize){
            movement.x = 0.0f;
        }
        //Outside left map
        if (nextPos.left < 0){
            movement.x = 0.0f;
        }


        //std::cout << playerBounds.top << playerBounds.height << mapH << mapBlockSize << std::endl;

        if (wallBounds.intersects(nextPos)){

            //Bottom collision
            if (playerBounds.top < wallBounds.top
            && playerBounds.top + playerBounds.height < wallBounds.top + wallBounds.height
            && playerBounds.left < wallBounds.left + wallBounds.width
            && playerBounds.left + playerBounds.width > wallBounds.left)
            {
                movement.y = 0.f;
            }

            //Top collision
            else if (playerBounds.top > wallBounds.top
            && playerBounds.top + playerBounds.height > wallBounds.top + wallBounds.height
            && playerBounds.left < wallBounds.left + wallBounds.width
            && playerBounds.left + playerBounds.width > wallBounds.left
            )
            {
                movement.y = 0.f;
            }

            //Right collision
            if (playerBounds.left < wallBounds.left
            && playerBounds.left + playerBounds.width < wallBounds.left + wallBounds.width
            && playerBounds.top < wallBounds.top + wallBounds.height
            && playerBounds.top + playerBounds.height > wallBounds.top
            )
            {
                movement.x = 0.f;
            }

            //Left collision
            else if (playerBounds.left > wallBounds.left
            && playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width
            && playerBounds.top < wallBounds.top + wallBounds.height
            && playerBounds.top + playerBounds.height > wallBounds.top
            )
            {
                movement.x = 0.f;
            }
        }
    }
    player->move(movement);
}
