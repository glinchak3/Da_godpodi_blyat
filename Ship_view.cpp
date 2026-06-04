#include "Ship_view.h"

// по умолчанию
ship_view::ship_view(){
    length = 1;
}

// с заданной длиной
ship_view::ship_view(int len){
    length = len;
}

// будем создавать спрайт из готовой текстуры (её создали в game)
void ship_view::set_texture(sf::Texture& tex, float scale){
    sprite.setTexture(tex);
    sprite.setScale(scale, scale);
    auto size = tex.getSize();
    sprite.setOrigin(0.f, 0.f);
}

// получает координаты корабля на поле В ПИКСЕЛЯХ
void ship_view::set_position(sf::Vector2f pos){
    sprite.setPosition(pos);
}

// отрисовывает корабль (один!)
void ship_view::draw(sf::RenderWindow& window){
    window.draw(sprite);
}

sf::Sprite& ship_view::get_sprite(){
    return sprite;
}

// получить позицию для ship в координатах сетки
std::vector<sf::Vector2i> ship_view::get_ship_cells(const board_view& board) const{
    std::vector<sf::Vector2i> cells;

    sf::Vector2i start = board.screen_to_cell(sprite.getPosition());

    for (int i = 0; i < length; i++){
        cells.push_back({start.x + i, start.y});
    }
    return cells;
}