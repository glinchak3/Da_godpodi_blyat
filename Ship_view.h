#pragma once

#include <SFML/Graphics.hpp>

class ship_view {
private:
    sf::Sprite sprite;
    bool vertical = false;

public:
    void setTexture(sf::Texture& tex, float scale);
    void setPosition(sf::Vector2f pos);
    void setRotation(bool v);
    void draw(sf::RenderWindow& window);

    sf::Sprite& getSprite();
    //sf::Vector2i getCell(const BoardView& board) const;
};