#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Ship_view.h"
#include "Board_view.h"

class ship_controller {
private:
    std::vector<ship_view> ships;

    int current_ship = 0;
    bool locked = false;
    bool vertical = false;

    BoardView* activeBoard = nullptr;
    bool ON = false;

public:
    bool get_ON();

    void add_ship(const ship_view& ship);
    void setBoard(BoardView* board);

    std::vector<sf::Vector2i> handle_event(
        const sf::Event& e,
        const sf::RenderWindow& window);

    void update(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    ship_view& get_current_ship();
};