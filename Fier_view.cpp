#include "Fier_view.h"

// загрузка текстуры
bool hit_view::load(const std::string& path)
{
    return texture.loadFromFile(path);
}

// добавление попадания
void hit_view::addHit(sf::Vector2i cell)
{
    hits.push_back(cell);
}

// отрисовка огоньков
void hit_view::draw(sf::RenderWindow& window, const BoardView& board)
{
    for (const auto& h : hits)
    {
        sf::Sprite fire;
        fire.setTexture(texture);

        // перевод клетки в пиксели
        sf::Vector2f pos = board.cellToScreen(h.x, h.y);
        fire.setPosition(pos);

        window.draw(fire);
    }
}