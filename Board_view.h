#pragma once

#include <SFML/Graphics.hpp>

class BoardView {
private:
    sf::Texture texture;
    sf::Sprite sprite;

public:
    bool load(const std::string& path, sf::Vector2f pos, sf::Vector2f windowSize);
    void draw(sf::RenderWindow& window);

    sf::FloatRect get_bounds() const;

    sf::Vector2f cellToScreen(int x, int y) const;
    sf::Vector2i screenToCell(sf::Vector2f pos) const;

    sf::Vector2i getCell(const BoardView& board) const;
};