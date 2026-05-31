#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Board_view.h"

class ship_view {
private:
    sf::Sprite sprite;
    bool vertical = false;
    int length;

public:
    ship_view();// конструктор по умолчанию
    ship_view(int len);// конструктор с длиной

    void setTexture(sf::Texture& tex, float scale);
    void setPosition(sf::Vector2f pos);
    void setRotation(bool v);
    void draw(sf::RenderWindow& window);

    sf::Sprite& getSprite();
    std::vector<sf::Vector2i> get_ship_cells(const BoardView& board) const;
};