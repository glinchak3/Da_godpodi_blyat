#include "Ship_controller.h"

bool ship_controller::get_ON(){
    return ON;
}

//добавление корабля в список визуальных кораблей
void ship_controller::add_ship(const ship_view& ship){ 
    ships.push_back(ship);
}

//устанавливает указатель на логическую доску
void ship_controller::set_board(board_view* board){
    active_board = board;
}

void ship_controller::set_game_board(board* board){
    game_board = board; 
}

std::vector<sf::Vector2i> ship_controller::handle_event(const sf::Event& e, const sf::RenderWindow& window){
    // Левый клик - захват/отпускание корабля
    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left){
        locked = !locked;
        return {};
    }

    // Клавиша Y - фиксация корабля
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Y){
        if (locked && current_ship < ships.size())
        {
            // Получаем клетки текущего корабля
            std::vector<sf::Vector2i> shipCells = ships[current_ship].get_ship_cells(*active_board);
            
            // Создаем временный корабль для проверки
            std::vector<cell> cells;
            for (const auto& pos : shipCells) {
                cells.push_back(cell(pos.x, pos.y));
            }

            ship temp_ship(shipCells.size(), cells);
            
            // Проверяем, можно ли поставить
            if (game_board && game_board->can_place_ship(temp_ship)){
                // Можно - фиксируем и переходим к следующему кораблю
                current_ship++;
                if (current_ship == ships.size()) {
                    ON = true;
                    locked = true;
                }
                else {
                    locked = false;
                }
                return shipCells;
            }
            else
            {
                locked = false;
                return {};  
            }
        }
        return {};
    }

    return {};
}

// обновление состояний корабля
void ship_controller::update(const sf::RenderWindow& window){

    if (!locked){ 
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f pos = window.mapPixelToCoords(mousePos);

        if (active_board){ 
            sf::Vector2i cell = active_board->screen_to_cell(pos);
            if (cell.x != -1){ 
                pos = active_board->cell_to_screen(cell.x, cell.y);
            }
        }

        ships[current_ship].set_position(pos);
    }
}

void ship_controller::draw(sf::RenderWindow& window){
    for (auto& s : ships)
        s.draw(window);
}

ship_view& ship_controller::get_current_ship(){
    return ships[current_ship];
}

