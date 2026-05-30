#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Board_view.h"
#include "Ship_controller.h"

#include "Board.h"
#include "Player.h"
#include "Ship.h"

class Game
{
private:
    sf::RenderWindow window;
    sf::Event event;

    BoardView playerBoardView;
    BoardView enemyBoardView;

    ShipController shipController;

    board playerBoard;
    board enemyBoard;

    Player* player = nullptr;
    Player* enemy = nullptr;

    void handle_events();
    void update();
    void render();

    void init();

public:
    Game();
    ~Game();

    void run();
};
