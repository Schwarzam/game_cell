//
// Created by gusta on 22/11/2021.
//

#include <fstream>
#include <iostream>
#include "Map.h"
#include "../Assets.h"
#include "../Collision.h"
#include "StringUtils.h"

std::vector<sf::Rect<float>> Map::walls;
candle::EdgeVector Map::edges;

Map::Map(const std::string& folder) {
    mapfolder = "maps/" + folder + "/";
    _ptexture = Assets::Acquire(mapfolder + "static");
    _sprite.setTexture(*_ptexture);
    readMetadata();
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
        if (nextPos.top + nextPos.height > metadata.at("height")){
            movement.y = 0.0f;
        }
        //Outside up map
        if (nextPos.top < 0){
            movement.y = 0.0f;
        }
        //Outside right map
        if (nextPos.left + nextPos.width > metadata.at("width")){
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

candle::EdgeVector Map::getEdges() {
    return edges;
}

void Map::readMetadata() {
    std::string myText;
    std::string mapName;

    // Read from the text file
    std::ifstream MyReadFile("assets/" + mapfolder + "metadata.txt");


    // Use a while loop together with the getline() function to read the file line by line
    while (getline (MyReadFile, myText)) {
        if (contains(myText, "//")){
            continue;
        }
        if (contains(myText, "--")){
            break;
        }
        if (myText.size() > 2){
            std::string index = myText.substr(0,myText.find(":"));
            if (index == "name"){
                mapName = index;
            }
            else if(index == "wall"){
                std::vector<std::string> v;
                split(trim(myText.substr(myText.find(":") + 1, myText.size())), ",", &v);

                float left = std::stof(v[0]);
                float top = std::stof(v[1]);
                float width = std::stof(v[2]);
                float height = std::stof(v[3]);

                walls.emplace_back(sf::Rect<float>(left, top, width, height));

                if (trim(v[4]) == "true"){
                    edges.emplace_back(sf::Vector2f(left, top), sf::Vector2f(left + width, top));
                    edges.emplace_back(sf::Vector2f(left, top), sf::Vector2f(left, top + height));
                    edges.emplace_back(sf::Vector2f(left + width, top), sf::Vector2f(left + width, top + height));
                    edges.emplace_back(sf::Vector2f(left, top + height), sf::Vector2f(left + width, top + height));
                }

            }else{
                metadata[index] = std::stof(trim(myText.substr(myText.find(":") + 1, myText.size())));
            }
        }
    }
}

Map::~Map() = default;
