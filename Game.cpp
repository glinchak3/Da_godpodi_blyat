#include <atomic>
#include <thread>
#include "Game.h"
#include <chrono>

Game::Game()
    : window(sf::VideoMode(800, 600), "Морской бой"){
    init();
}

Game::~Game(){
    is_running = false;
    if (renderThread.joinable()) {
        renderThread.join();
    }
}

void Game::init(){
    std::srand(std::time(nullptr));


    // создаём доски с помощью board_view
    playerBoardView.load("images/Field_1.png", {0.f, 0.f}, (sf::Vector2f)window.getSize());
    enemyBoardView.load("images/Field_2.png", {425.f, 0.f}, (sf::Vector2f)window.getSize());

    // загружаем корабли
    t4.loadFromFile("images/b_4.png");
    t3.loadFromFile("images/b_3.png");
    t2.loadFromFile("images/b_2.png");
    t1.loadFromFile("images/b_1.png");

    // 1 четырёхпалубник
    {
        ship_view ship(4);
        ship.set_texture(t4, 0.108f);
        shipController.add_ship(ship);
    }

    // 2 трёхпалубника
    for (int i = 0; i < 2; i++)
    {
        ship_view ship(3);
        ship.set_texture(t3, 0.08f);
        shipController.add_ship(ship);
    }

    // 3 двухпалубника
    for (int i = 0; i < 3; i++)
    {
        ship_view ship(2);
        ship.set_texture(t2, 0.08f);
        shipController.add_ship(ship);
    }

    // 4 однопалубника
    for (int i = 0; i < 4; i++)
    {
        ship_view ship(1);
        ship.set_texture(t1, 0.08f);
        shipController.add_ship(ship);
    }

    win_texture.loadFromFile("images/win.png");
    lose_texture.loadFromFile("images/lose.png");
    
    win_sprite.setTexture(win_texture);
    lose_sprite.setTexture(lose_texture);

    win_sprite.setPosition(100.f, 100.f);
    lose_sprite.setPosition(100.f, 100.f);

    shipController.set_board(&playerBoardView);
    shipController.set_game_board(&player_board);

    player = new HumanPlayer(player_board, enemy_board);
    enemy = new ComputerPlayer(enemy_board, player_board);

    player_shots.load_hit("images/Fier.png");
    player_shots.load_miss("images/Miss.png");
    enemy_shots.load_hit("images/Fier.png");
    enemy_shots.load_miss("images/Miss.png");

    player_shots.set_hit_scale(0.05f);
    player_shots.set_miss_scale(0.03f);
    enemy_shots.set_hit_scale(0.05f);
    enemy_shots.set_miss_scale(0.03f);

    // Автоматическая расстановка всего флота робота на старте
    enemy->place_ship(ship(0, {}));

    window.setActive(false); 
}

void Game::handle_events(){
    while (window.pollEvent(event)){
        if (event.type == sf::Event::Closed) {
            window.close();
            is_running = false;
        }

        if (state == game_state::placement) {
            std::vector<sf::Vector2i> coords_list = shipController.handle_event(event, window);
            
            if (!coords_list.empty()) {
                ship new_ship(coords_list);
                player->place_ship(new_ship);
                
                if (shipController.get_ON()) {
                    state = game_state::player_turn;
                }
            }
        }
    }
}

void Game::update(){
    if (state == game_state::placement) {
        shipController.update(window);
    }
}

void Game::render(){
    window.clear();
    
    if (state == game_state::game_over) {
        if (player_won)
            window.draw(win_sprite);
        else
            window.draw(lose_sprite);
    
        window.display();
        return;
    }

    playerBoardView.draw(window);
    enemyBoardView.draw(window);
    shipController.draw(window);
    player_shots.draw(window, playerBoardView);
    enemy_shots.draw(window, enemyBoardView);
    window.display();
}

void Game::run(){
    window.setActive(false);

    while (window.isOpen() && is_running){
        handle_events();
        update();
        render();
        
        if (state == game_state::player_turn && 
            event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left){
            
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f coords = window.mapPixelToCoords(mousePos);
            sf::Vector2i targetedCell = enemyBoardView.screen_to_cell(coords);
            
            if (targetedCell.x != -1){
                cell shotCell(targetedCell.x, targetedCell.y);
                if (enemy_board.can_shoot(shotCell)){
                    bool hit = enemy_board.shoot(shotCell);
                    if (hit){
                        enemy_shots.add_hit(sf::Vector2i(shotCell.get_x(), shotCell.get_y()));
                    } else {
                        enemy_shots.add_miss(sf::Vector2i(shotCell.get_x(), shotCell.get_y()));
                    }
                    
                    if (enemy_board.victory()) {
                        player_won = true;
                        state = game_state::game_over;
                    } else if (!hit) {
                        state = game_state::enemy_turn;
                    }
                }
            }
        } 
        else if (state == game_state::enemy_turn) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            cell enemyShot = enemy->take_turn();
            
            bool hit = player_board.shoot(enemyShot);
            
            if (hit) {
                player_shots.add_hit(enemyShot.to_sf());
            } else {
                player_shots.add_miss(enemyShot.to_sf());
            }

            if (player_board.victory()) {
                player_won = false;
                state = game_state::game_over;
            } else if (!hit) {
                state = game_state::player_turn;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}