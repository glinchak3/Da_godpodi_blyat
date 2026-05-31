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

public:
    void add_ship(const ship_view& ship);

    void setBoard(BoardView* board);

    std::vector<sf::Vector2i> handle_event(const sf::Event& e, const sf::RenderWindow& window);
    void update(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    std::vector<sf::Vector2i>& get_current_ship();
};