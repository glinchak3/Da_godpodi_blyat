#pragma once

#include <SFML/Graphics.hpp>

class BoardView {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::FloatRect bounds;

    sf::Vector2f boardOrigin;
float cellSize;
    
    sf::Vector2f offset{0.f, 0.f};
    void setOffset(sf::Vector2f off);

    sf::Vector2f gridOffset{ -2.f, 1.f };

public:
    bool load(const std::string& path, sf::Vector2f pos, sf::Vector2f windowSize);
    void draw(sf::RenderWindow& window);

    sf::FloatRect get_bounds() const;

    sf::Vector2f cellToScreen(int x, int y) const;
    sf::Vector2i screenToCell(sf::Vector2f pos) const;

    void setParams(sf::Vector2f origin, float cellSize);

   
};