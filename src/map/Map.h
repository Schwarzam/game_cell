//
// Created by gusta on 22/11/2021.
//

#ifndef THECELL_MAP_H
#define THECELL_MAP_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "../Player.h"
#include "../utils/Candle/include/Candle/LightSource.hpp"

class Map : public sf::Drawable {
public:
    explicit Map(const std::string& name);
    static void move(Player *player, sf::Vector2f movement);

    static candle::EdgeVector getEdges();
private:
    void readMetadata();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    std::shared_ptr<sf::Texture> _ptexture;
    sf::Sprite _sprite;

    std::string mapfolder;

    static std::map<std::string, float> metadata;

    static std::vector<sf::Rect<float>> walls;
    static candle::EdgeVector edges;
};




#endif //THECELL_MAP_H
