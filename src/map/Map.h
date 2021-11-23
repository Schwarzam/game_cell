//
// Created by gusta on 22/11/2021.
//

#ifndef THECELL_MAP_H
#define THECELL_MAP_H

#include <nlohmann/json.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class Map : public sf::Drawable {
public:
    explicit Map(std::string name);

    static bool validadePos(sf::Sprite&, sf::Vector2f);

    void loadNextMap();

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    std::shared_ptr<sf::Texture> _ptexture;
    sf::Sprite _sprite;

    std::string mapfolder;
    static nlohmann::json map_metadata;
    static std::vector<sf::Rect<float>> walls;
};




#endif //THECELL_MAP_H
