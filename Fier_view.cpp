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
    // огонь
    for (auto& h : hits)
    {
        sf::Sprite s;
        s.setTexture(hitTexture);
        s.setScale(hitScale, hitScale);
        s.setPosition(board.cellToScreen(h.x, h.y));
        window.draw(s);
    }

    // крестик
    for (auto& m : misses)
    {
        sf::Sprite s;
        s.setTexture(missTexture);
        s.setScale(missScale, missScale);
        s.setPosition(board.cellToScreen(m.x, m.y));
        window.draw(s);
    }
}

void ShotView::setHitScale(float scale) {
    hitScale = scale;
}

void ShotView::setMissScale(float scale) {
    missScale = scale;
}