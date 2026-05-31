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
    enemyBoardView.load("images/Field_2.png", {425.f, 0.f}, (sf::Vector2f)window.getSize());

    // загружаем корабли
    t4.loadFromFile("images/b_4.png");
    t3.loadFromFile("images/b_3.png");
    t2.loadFromFile("images/b_2.png");
    t1.loadFromFile("images/b_1.png");

    // 1 четырёхпалубник
    {
        ship_view ship;
        ship.setTexture(t4, 0.108f);
        shipController.add_ship(ship);
    }

    // 2 трёхпалубника
    for (int i = 0; i < 2; i++)
    {
        ship_view ship;
        ship.setTexture(t3, 0.08f);
        shipController.add_ship(ship);
    }

    // 3 двухпалубника
    for (int i = 0; i < 3; i++)
    {
        ship_view ship;
        ship.setTexture(t2, 0.08f);
        shipController.add_ship(ship);
    }

    // 4 однопалубника
    for (int i = 0; i < 4; i++)
    {
        ship_view ship;
        ship.setTexture(t1, 0.08f);
        shipController.add_ship(ship);
    }

        shipController.setBoard(&playerBoardView);

        //player = new Player(playerBoard, enemyBoard);
        //enemy  = new Player(enemyBoard, playerBoard);

        fier_view.load("images/Fire.png");
    }

    fier_view.addHit(cell); // СШИТЬ этой штуке нужно передать куда попало

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

    hitView.draw(window, playerBoardView)

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

    