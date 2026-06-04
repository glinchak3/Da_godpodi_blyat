#include "Fier_view.h"

bool ShotView::loadHit(const std::string& path)
{
    return hitTexture.loadFromFile(path);
}

bool ShotView::loadMiss(const std::string& path)
{
    return missTexture.loadFromFile(path);
}

void ShotView::addHit(sf::Vector2i cell)
{
    hits.push_back(cell);
}

void ShotView::addMiss(sf::Vector2i cell)
{
    misses.push_back(cell);
}

void ShotView::draw(sf::RenderWindow& window, const BoardView& board)
{
    // Получаем размеры клетки
    sf::FloatRect bounds = board.get_bounds();
    float cellW = bounds.width / 11.f;
    float cellH = bounds.height / 11.f;
    
    // огонь
    for (auto& h : hits)
    {
        sf::Sprite s;
        s.setTexture(hitTexture);
        s.setScale(hitScale, hitScale);
        sf::Vector2f cellPos = board.cellToScreen(h.x, h.y);
        s.setPosition(cellPos.x + 4.f, cellPos.y +  2.f);
        window.draw(s);
    }

    // крестик
    for (auto& m : misses)
    {
        sf::Sprite s;
        s.setTexture(missTexture);
        s.setScale(missScale, missScale);
        sf::Vector2f cellPos = board.cellToScreen(m.x, m.y);
        s.setPosition(cellPos.x +  1.f, cellPos.y +  1.f);
        window.draw(s);
    }
}

void ShotView::setHitScale(float scale) {
    hitScale = scale;
}

void ShotView::setMissScale(float scale) {
    missScale = scale;
}