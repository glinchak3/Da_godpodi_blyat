#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Ship_view.h"
#include "Board_view.h"
#include "Board.h"

class ship_controller {
private:
    std::vector<ship_view> ships;

    int current_ship = 0;
    bool locked = false; //зафиксирован ли корабль на поле

    board_view* active_board = nullptr; // указатель графическую доску
    board* game_board = nullptr; // указатель на логическую доску
    bool ON = false; // флаг окончания расстановки кораблей

public:
    //устанавливает указатель на логическую доску
    void set_game_board(board* board);
    
    //устанавливает указатель на логическую доску
    void set_board(board_view* board);

    bool get_ON();

    void add_ship(const ship_view& ship);

   // обрабатывает ввод пользователя
    std::vector<sf::Vector2i> handle_event(const sf::Event& e, const sf::RenderWindow& window);

    // обновляет позицию текущего корабля
    void update(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    ship_view& get_current_ship();
};