#pragma once

#include <SFML/Graphics.hpp>

class board_view {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::FloatRect bounds; //границы спрайта

    sf::Vector2f board_origin;

public:
    bool load(const std::string& path, sf::Vector2f pos, sf::Vector2f windowSize);
    void draw(sf::RenderWindow& window);

    sf::FloatRect get_bounds() const;

    sf::Vector2f cell_to_screen(int x, int y) const;
    sf::Vector2i screen_to_cell(sf::Vector2f pos) const;

   
};