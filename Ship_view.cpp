#include "Ship_view.h"
// будем создавать спрайт из готовой текстуры (её создали в game)
void ship_view::setTexture(sf::Texture& tex, float scale){
    sprite.setTexture(tex);
    sprite.setScale(scale, scale);

    auto size = tex.getSize();
    sprite.setOrigin(size.x / 2.f, size.y / 2.f);
}

// получает координаты корабля на поле В ПИКСЕЛЯХ
// Логика такая: контроллер через мышь получает пиксели и перевоодит в координаты
// Board_view переводит обратно
// Дальше он уже ставит
void ship_view::setPosition(sf::Vector2f pos){
    sprite.setPosition(pos);
}

void ship_view::setRotation(bool v){
    vertical = v;
    sprite.setRotation(vertical ? 90.f : 0.f);
}

//отрисовывает корабль (один!)
void ship_view::draw(sf::RenderWindow& window){
    window.draw(sprite);
}

sf::Sprite& ship_view::getSprite(){
    return sprite;
}