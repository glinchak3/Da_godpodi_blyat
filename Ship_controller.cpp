#include "Ship_controller.h"

void ship_controller::add_ship(const ship_view& ship){
    ships.push_back(ship);
}

void ship_controller::setBoard(BoardView* board){
    activeBoard = board;
}

void ship_controller::handle_event(const sf::Event& e, const sf::RenderWindow& window){

    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left){
        locked = !locked;
    }

    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Y){
        if (locked){
            current_ship++;

            if (current_ship >= (int)ships.size())
                current_ship = ships.size() - 1;

            locked = false;
        }
    }

    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::R){
        vertical = !vertical;
        ships[current_ship].setRotation(vertical);
    }
}

void ship_controller::update(const sf::RenderWindow& window){
    if (!locked){
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f pos = window.mapPixelToCoords(mousePos);

        if (activeBoard){
            sf::Vector2i cell = activeBoard->screenToCell(pos);

            if (cell.x != -1){
                pos = activeBoard->cellToScreen(cell.x, cell.y);
            }
        }

        ships[current_ship].setPosition(pos);
    }
}

void ship_controller::draw(sf::RenderWindow& window){
    for (auto& s : ships)
        s.draw(window);
}

ship_view& ship_controller::get_current_ship(){
    return ships[current_ship];
}