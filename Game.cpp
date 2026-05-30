#include "Game.h"

Game::Game()
    : window(sf::VideoMode(800, 600), "Морской бой"){
    init();
}

Game::~Game(){
    delete player;
    delete enemy;
}

void Game::init(){

    playerBoardView.load("images/Field.png", {250.f, 300.f}, (sf::Vector2f)window.getSize());
    enemyBoardView.load("images/Field.png", {550.f, 300.f}, (sf::Vector2f)window.getSize());

    sf::Texture t4, t3, t2, t1;

    t4.loadFromFile("images/b_4.png");
    t3.loadFromFile("images/b_3.png");
    t2.loadFromFile("images/b_2.png");
    t1.loadFromFile("images/b_1.png");

    ship_view s1; s1.setTexture(t4, 0.079f);
    ship_view s2; s2.setTexture(t3, 0.0605f);
    ship_view s3; s3.setTexture(t2, 0.056f);
    ship_view s4; s4.setTexture(t1, 0.055f);

    shipController.add_ship(s1);
    shipController.add_ship(s2);
    shipController.add_ship(s3);
    shipController.add_ship(s4);

    shipController.setBoard(&playerBoardView);

    player = new Player(playerBoard, enemyBoard);
    enemy  = new Player(enemyBoard, playerBoard);
}

void Game::handle_events(){
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab)
            shipController.setBoard(&enemyBoardView);

        shipController.handle_event(event, window);
    }
}

void Game::update(){
    shipController.update(window);
}
void Game::run(){
    std::thread t3()
    while((human_board.victory() == false)&&(computer_board.victory() == false)){
        if (human_turn == true){
            cell c = player.take_turn();
            human_turn = false;
            computer_turn = true;
        } else if (computer_turn == true) {
            enemy.take_turn();
            computer_turn = false;
            human_turn = true;
        }
    }
}
void Game::render(){
    window.clear();

    playerBoardView.draw(window);
    enemyBoardView.draw(window);

    shipController.draw(window);

    window.display();
}