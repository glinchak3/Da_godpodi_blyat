#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Board_view.h"  // изменить с Board_view.h на board_view.h

class ShotView {
private:
    sf::Texture hit_texture;   // изменить hitTexture → hit_texture
    sf::Texture miss_texture;  // изменить missTexture → miss_texture
    std::vector<sf::Vector2i> hits;
    std::vector<sf::Vector2i> misses;
    float hit_scale = 0.0005f;
    float miss_scale = 0.0005f;

public:
    bool load_hit(const std::string& path);
    bool load_miss(const std::string& path);
    void add_hit(sf::Vector2i cell);
    void add_miss(sf::Vector2i cell);
    void draw(sf::RenderWindow& window, const board_view& board);  // BoardView → board_view
    
    void set_hit_scale(float scale);
    void set_miss_scale(float scale);  // исправить set_missScale → set_miss_scale
};