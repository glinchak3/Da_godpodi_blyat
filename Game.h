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

    float cellSize = 40.f;
float boardSize = cellSize * 10.f;

    BoardView playerBoardView;
    BoardView enemyBoardView;

    ship_controller shipController;

    board playerBoard;
    board enemyBoard;

    sf::Texture t4, t3, t2, t1;

   // Player* player = nullptr;
   // Player* enemy = nullptr;

    void handle_events();
    void update();
    void render();

    void init();

public:
    Game();
    //~Game();

    void run();
};