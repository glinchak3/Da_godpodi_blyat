#include "Game.h"

Game::Game()
    : window(sf::VideoMode(800, 600), "Морской бой"){
    init();
}

//Game::~Game(){
  // delete player;
    //delete enemy;
//}

void Game::init(){
    // создаём доски с помощью board_view
    playerBoardView.load("images/Field_1.png", {0.f, 0.f}, (sf::Vector2f)window.getSize());
    enemyBoardView.load("images/Field_2.png", {0.f, 0.f}, (sf::Vector2f)window.getSize());

    // ✔ НОРМАЛЬНОЕ РАСПОЛОЖЕНИЕ (без магии)
    playerBoardView.setParams({50.f, 50.f}, cellSize);
    enemyBoardView.setParams({50.f + boardSize + 50.f, 50.f}, cellSize);

    // загружаем корабли
    t4.loadFromFile("images/b_4.png");
    t3.loadFromFile("images/b_3.png");
    t2.loadFromFile("images/b_2.png");
    t1.loadFromFile("images/b_1.png");

    ship_view s1; s1.setTexture(t4, 0.11f);
    ship_view s2; s2.setTexture(t3, 0.07f);
    ship_view s3; s3.setTexture(t2, 0.056f);
    ship_view s4; s4.setTexture(t1, 0.055f);

    shipController.add_ship(s1);
    shipController.add_ship(s2);
    shipController.add_ship(s3);
    shipController.add_ship(s4);

    shipController.setBoard(&playerBoardView);

    //player = new Player(playerBoard, enemyBoard);
    //enemy  = new Player(enemyBoard, playerBoard);
}

void Game::handle_events()
{
    while (window.pollEvent(event)){
        if (event.type == sf::Event::Closed)
            window.close();
        shipController.handle_event(event, window);
    }
}

void Game::update(){
    shipController.update(window);
}

void Game::render(){
    window.clear();

    playerBoardView.draw(window);
    enemyBoardView.draw(window);

    shipController.draw(window);

    window.display();
}

void Game::run()
{
    while (window.isOpen())
    {
        handle_events();
        update();
        render();
    }
}

    