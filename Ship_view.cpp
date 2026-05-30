#include "Ship_view.h"

void ship_view::setTexture(sf::Texture& tex, float scale){
    sprite.setTexture(tex);
    sprite.setScale(scale, scale);

    auto size = tex.getSize();
    sprite.setOrigin(size.x / 2.f, size.y / 2.f);
}

void ship_view::setPosition(sf::Vector2f pos){
    sprite.setPosition(pos);
}

void ship_view::setRotation(bool v){
    vertical = v;
    sprite.setRotation(vertical ? 90.f : 0.f);
}

void ship_view::draw(sf::RenderWindow& window){
    window.draw(sprite);
}

sf::Sprite& ship_view::getSprite(){
    return sprite;
}