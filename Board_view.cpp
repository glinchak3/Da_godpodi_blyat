#include "Board_view.h"

//загрузка и настройка изображения
bool BoardView::load(const std::string& path, sf::Vector2f pos, sf::Vector2f windowSize){
    sf::Image img;
    if (!img.loadFromFile(path))
        return false;

    texture.loadFromImage(img);
    sprite.setTexture(texture);

    auto size = texture.getSize();

    float scale = 0.40f;

    sprite.setScale(scale, scale);
    sprite.setOrigin(0.f, 0.f);
    sprite.setPosition(pos);

    bounds = sprite.getGlobalBounds();
    return true;
}

//отрисовка картинки
void BoardView::draw(sf::RenderWindow& window){
    window.draw(sprite);
}

//получение границ картинки
sf::FloatRect BoardView::get_bounds() const{
    return bounds;
}

//перевод сетки из координат в сетку из пикселей 
sf::Vector2f BoardView::cellToScreen(int x, int y) const
{
    sf::FloatRect b = sprite.getGlobalBounds();

    float cellW = b.width / 11.f;
    float cellH = b.height / 11.f;

    return {b.left + x * cellW, b.top  + y * cellH
    };
}

//перевод сетки из пикселей в сетку из координат
sf::Vector2i BoardView::screenToCell(sf::Vector2f pos) const
{
    sf::FloatRect b = sprite.getGlobalBounds();

    if (!b.contains(pos))
        return {-1, -1};

    float cellW = b.width / 11.f;
    float cellH = b.height / 11.f;

    int x = int((pos.x - b.left) / cellW);
    int y = int((pos.y - b.top)  / cellH);

    return {x, y};
}
