#include <SFML/Graphics.hpp>
#include <vector>
#include "Ship.h"
#include "Cell.h"

std::vector<sf::Sprite> ships;
std::vector<sf::Texture> textures;

sf::Sprite create_ship(sf::Texture& tex, float scale){
    sf::Sprite s;
    s.setTexture(tex);
    s.setScale(scale, scale);

    sf::Vector2u size = tex.getSize();
    s.setOrigin(size.x / 2.f, size.y / 2.f);

    return s;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Морской бой");

    //Поле
    sf::Image field_image;
    if (!field_image.loadFromFile("images/Field.png"))
        return -1;

    sf::Texture field_texture;
    field_texture.loadFromImage(field_image);

    sf::Sprite field_sprite;
    field_sprite.setTexture(field_texture);

    sf::Vector2u tex_size = field_texture.getSize();
    sf::Vector2u win_size = window.getSize();

    float scale = std::min(
        (float)win_size.x / tex_size.x,
        (float)win_size.y / tex_size.y
    );

    field_sprite.setScale(scale, scale);
    field_sprite.setOrigin(tex_size.x / 2.f, tex_size.y / 2.f);
    field_sprite.setPosition(400.f, 300.f);

    //Корабли
    sf::Texture ship_4_tex, ship_3_tex, ship_2_tex, ship_1_tex;

    ship_4_tex.loadFromFile("images/b_4.png");
    ship_3_tex.loadFromFile("images/b_3.png");
    ship_2_tex.loadFromFile("images/b_2.png");
    ship_1_tex.loadFromFile("images/b_1.png");

    textures = {ship_4_tex, ship_3_tex, ship_2_tex, ship_1_tex};

    // 1 × 4
    ships.push_back(create_ship(textures[0], 0.079f));

    // 2 × 3
    ships.push_back(create_ship(textures[1], 0.0605f));
    ships.push_back(create_ship(textures[1], 0.0605f));

    // 3 × 2
    ships.push_back(create_ship(textures[2], 0.056f));
    ships.push_back(create_ship(textures[2], 0.056f));
    ships.push_back(create_ship(textures[2], 0.056f));

    // 4 × 1
    ships.push_back(create_ship(textures[3], 0.055f));
    ships.push_back(create_ship(textures[3], 0.055f));
    ships.push_back(create_ship(textures[3], 0.055f));
    ships.push_back(create_ship(textures[3], 0.055f));


    int current_ship = 0;
    bool locked = false;
    bool vertical = false;

    ships[0].setPosition(100.f, 100.f);

    while (window.isOpen()){
        sf::Event event;

        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();

            // ЛКМ - закрепить или открепить
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                locked = !locked;

            // Y - следующий корабль
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Y){
                if (locked){
                    current_ship++;

                    if (current_ship >= (int)ships.size())
                        current_ship = ships.size() - 1;

                        locked = false;
                }
            }

            // поворот
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R){
                vertical = !vertical;
            }
        }

        ships[current_ship].setRotation(vertical ? 90.f : 0.f);

        //движение
        if (!locked){
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            ships[current_ship].setPosition((float)mousePos.x, (float)mousePos.y);
        }

        window.clear();
        window.draw(field_sprite);

        for (auto& s : ships)
            window.draw(s);

        window.display();
    }

    return 0;
}