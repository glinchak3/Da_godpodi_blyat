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
    ShotView player_shots;  // shot_view → ShotView
    ShotView enemy_shots;   // shot_view → ShotView

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

    std::atomic<bool> is_running{ true };
    std::atomic<game_state> state{ game_state::placement };  // GameState → game_state
    bool player_won = false;

    void handle_events();
    void update();
    void render();
    void init();

    std::thread renderThread; 

public:
    Game();
    ~Game();
    void run();
};