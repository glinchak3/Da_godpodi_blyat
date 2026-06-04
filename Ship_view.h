#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Board_view.h"

class ship_view {
private:
    sf::Sprite sprite;
    int length;

public:
    ship_view();
    ship_view(int len);

    void set_texture(sf::Texture& tex, float scale);
    void set_position(sf::Vector2f pos);
    void draw(sf::RenderWindow& window);
    sf::Sprite& get_sprite();
    std::vector<sf::Vector2i> get_ship_cells(const board_view& board) const;
};