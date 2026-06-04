#include "Ship_controller.h"

bool ship_controller::get_ON(){
    return ON;
}

//добавление корабыль в список визуальных кораблей
void ship_controller::add_ship(const ship_view& ship){ 
    ships.push_back(ship);
}

// задаём о какой доске мы сейчас говорим
void ship_controller::setBoard(BoardView* board){
    activeBoard = board;
}

std::vector<sf::Vector2i> ship_controller::handle_event(
    const sf::Event& e,
    const sf::RenderWindow& window)
{
    // Левый клик - захват/отпускание корабля
    if (e.type == sf::Event::MouseButtonPressed &&
        e.mouseButton.button == sf::Mouse::Left)
    {
        locked = !locked;
        return {};
    }

    

    // Клавиша Y - фиксация корабля
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Y)
    {
        if (locked && current_ship < ships.size())
        {
            // Получаем клетки текущего корабля
            std::vector<sf::Vector2i> shipCells = ships[current_ship].get_ship_cells(*activeBoard);
            
            // Создаем временный корабль для проверки
            std::vector<cell> cells;
            for (const auto& pos : shipCells) {
                cells.push_back(cell(pos.x, pos.y));
            }
            ship tempShip(shipCells.size(), cells);
            
            // Проверяем, можно ли поставить
            if (gameBoard && gameBoard->can_place_ship(tempShip))
            {
                // Можно - фиксируем и переходим к следующему кораблю
                current_ship++;
                if (current_ship == ships.size()) {
                    ON = true;      // Все корабли расставлены
                    locked = true;
                } else {
                    locked = false;  // Разблокируем следующий корабль
                }
                return shipCells;    // Возвращаем клетки для размещения на доске
            }
            else
            {
                // Нельзя поставить - сбрасываем locked, чтобы можно было двигать дальше
                locked = false;
                return {};           // Возвращаем пустой вектор - фиксация не удалась
            }
        }
        return {};
    }

    return {};
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