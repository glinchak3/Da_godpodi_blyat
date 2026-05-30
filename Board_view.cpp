#include "Board_view.h"

bool BoardView::load(const std::string& path, sf::Vector2f pos, sf::Vector2f windowSize){
    sf::Image img;
    if (!img.loadFromFile(path))
        return false;

    texture.loadFromImage(img);
    sprite.setTexture(texture);

    auto size = texture.getSize();

    float scale = std::min(windowSize.x / size.x, windowSize.y / size.y);

    sprite.setScale(scale, scale);
    sprite.setOrigin(size.x / 2.f, size.y / 2.f);
    sprite.setPosition(pos);

    bounds = sprite.getGlobalBounds();
    return true;
}

void BoardView::draw(sf::RenderWindow& window){
    window.draw(sprite);
}

sf::FloatRect BoardView::get_bounds() const{
    return bounds;
}

sf::Vector2f BoardView::cellToScreen(int x, int y) const
{
    sf::FloatRect b = sprite.getGlobalBounds();

    float cellW = b.width / 10.f;
    float cellH = b.height / 10.f;

    return {
        b.left + x * cellW + cellW / 2.f,
        b.top  + y * cellH + cellH / 2.f
    };
}

sf::Vector2i BoardView::screenToCell(sf::Vector2f pos) const
{
    sf::FloatRect b = sprite.getGlobalBounds();

    if (!b.contains(pos))
        return {-1, -1};

    float cellW = b.width / 10.f;
    float cellH = b.height / 10.f;

    int x = int((pos.x - b.left) / cellW);
    int y = int((pos.y - b.top)  / cellH);

    return {x, y};
}