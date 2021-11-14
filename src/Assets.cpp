//
// Created by gusta on 13/11/2021.
//

#include "Assets.h"
#include <iostream>
#include "Collision.h"

std::unordered_map<std::string, std::shared_ptr<sf::Texture>> Assets::textures;

std::shared_ptr<sf::Texture> Assets::Acquire(const std::string &name) {
    const auto i = textures.find( name );
    if( i != textures.end() ){
        return i->second;
    }
    else{
        auto pTex = std::make_shared<sf::Texture>();
        Collision::CreateTextureAndBitmask(*pTex, "media/" + name + ".png");
        textures.insert({name, pTex});
        return pTex;
    }
}

void Assets::Murder() {
    for (auto i = textures.begin(); i != textures.end();){
        if (i->second.unique()){
            i = textures.erase(i);
        }
    }
}