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
void ship_controller::handle_event(const sf::Event& e, const sf::RenderWindow& window){
    //переключение мышью закреплено или откреплено
    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left){
        locked = !locked;
    }

    // меняем активный корабль на новый при окончательном закреплении предыдущего
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Y){
        if (locked){
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
}

// обновление состояний корабля
void ship_controller::update(const sf::RenderWindow& window){
    // если не зафиксилован то можно двигать
    if (!locked){ 
        sf::Vector2i mousePos = sf::Mouse::getPosition(window); // берём координаты мыши в пикселях
        sf::Vector2f pos = window.mapPixelToCoords(mousePos); // переводим в координаты sfml
        if (activeBoard){ // если доска есть
            sf::Vector2i cell = activeBoard->screenToCell(pos); // переводим в координаты сетки

            if (cell.x != -1){ // если мы на поле
                pos = activeBoard->cellToScreen(cell.x, cell.y); //перевод снова в клетки, ставим корабль именно в центр
            }
        }

        ships[current_ship].setPosition(pos); // собственно, ставим корабль
    }
}

void ship_controller::draw(sf::RenderWindow& window){
    for (auto& s : ships)
        s.draw(window);
}

ship_view& ship_controller::get_current_ship(){
    return ships[current_ship];
}