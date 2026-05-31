#include "Ship_controller.h"

//добавление корабыль в список визуальных кораблей
void ship_controller::add_ship(const ship_view& ship){ 
    ships.push_back(ship);
}

// задаём о какой доске мы сейчас говорим
void ship_controller::setBoard(BoardView* board){
    activeBoard = board;
}

//реагирует на ввод игрока
cell ship_controller::handle_event(const sf::Event& e, const sf::RenderWindow& window){

    //переключение мышью закреплено или откреплено
    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left){
        locked = !locked;
    }

    // меняем активный корабль на новый при окончательном закреплении предыдущего
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Y){
        if (locked){
            // МЕСТО СШИВКИ
            sf::Vector2i ship_cell = ships[current_ship].getCell(*activeBoard);//получаем клетку для корабля
            current_ship++;

            if (current_ship >= (int)ships.size())
                current_ship = ships.size() - 1;

            locked = false;
        }
    }

    // поворот
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::R){
        vertical = !vertical;
        ships[current_ship].setRotation(vertical);
    }
    return cell(ship_cell.x,ship_cell.y);
}

// обновление состояний корабля
void ship_controller::update(const sf::RenderWindow& window){

    // если не зафиксилован то можно двигать
    if (!locked){ 

        sf::Vector2i mousePos = sf::Mouse::getPosition(window); // пиксели мыши
        sf::Vector2f pos = window.mapPixelToCoords(mousePos);   // world coords

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
