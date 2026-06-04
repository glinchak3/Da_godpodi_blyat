#pragma once

#include <atomic>
#include <thread>
#include "Board_view.h"
#include "Ship_controller.h"
#include "Fier_view.h"
#include "Board.h"
#include "Player.h"
#include "Ship.h"

enum class game_state {
    placement,
    player_turn,
    enemy_turn,
    game_over
};

class Game {
private:
    ShotView player_shots;
    ShotView enemy_shots;

    sf::RenderWindow window;
    sf::Event event;

    board_view playerBoardView;
    board_view enemyBoardView;

    ship_controller shipController;

    board player_board;
    board enemy_board;

    sf::Texture t4, t3, t2, t1;
    sf::Texture win_texture, lose_texture;
    sf::Sprite win_sprite, lose_sprite;

    HumanPlayer* player;
    ComputerPlayer* enemy;

    bool is_running = true;
    game_state state = game_state::placement;
    bool player_won = false;

    void init(); // инициализация
    void handle_events(); //обработка событий
    void update(); // обновление состояний
    void render(); // визуализация

public:
    Game();
    ~Game();
    void run();
};