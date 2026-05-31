#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Board_view.h"

class hit_view
{
private:
    sf::Texture texture;
    std::vector<sf::Vector2i> hits;

public:
    // загрузка текстуры огонька
    bool load(const std::string& path);

    // добавить попадание
    void addHit(sf::Vector2i cell);

    // отрисовка всех огоньков
    void draw(sf::RenderWindow& window, const BoardView& board);
};