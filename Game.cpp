#include "Game.h"
#include <chrono>

Game::Game()
    : window(sf::VideoMode(800, 600), "Морской бой"){
    init();
}

Game::~Game(){
    isRunning = false; // Сигнал для остановки потока отрисовки
    if (renderThread.joinable()) {
        renderThread.join(); // Ждем завершения графического потока
    }
}

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
        ship_view ship(4);
        ship.setTexture(t4, 0.108f);
        shipController.add_ship(ship);
    }

    // 2 трёхпалубника
    for (int i = 0; i < 2; i++)
    {
        ship_view ship(3);
        ship.setTexture(t3, 0.08f);
        shipController.add_ship(ship);
    }

    // 3 двухпалубника
    for (int i = 0; i < 3; i++)
    {
        ship_view ship(2);
        ship.setTexture(t2, 0.08f);
        shipController.add_ship(ship);
    }

    // 4 однопалубника
    for (int i = 0; i < 4; i++)
    {
        ship_view ship(1);
        ship.setTexture(t1, 0.08f);
        shipController.add_ship(ship);
    }

    shipController.setBoard(&playerBoardView);

    player = new HumanPlayer(playerBoard, enemyBoard);
    enemy  = new ComputerPlayer(enemyBoard, playerBoard);

    fier_view.load("images/Fire.png");

    // Автоматическая расстановка всего флота робота на старте
    enemy->place_ship(ship(0, {}));

    // ВАЖНО ДЛЯ ПОТОКОВ: Отключаем графический контекст окна в текущем (главном) потоке
    window.setActive(false); 
}


void Game::handle_events()
{
    while (window.pollEvent(event)){
        if (event.type == sf::Event::Closed) {
            window.close();
            isRunning = false;
        }

        if (state == GameState::Placement) {
            std::vector<sf::Vector2i> coords_list = shipController.handle_event(event, window);
            
            // Если вектор координат не пустой — значит, игрок успешно зафиксировал корабль на карте
            if (!coords_list.empty()) {
                ship new_ship(coords_list);
                player->place_ship(new_ship); // ИСПРАВЛЕНО: Обращение через указатель ->

                // Проверяем автоматический переход к бою: если расставлено 10 кораблей
            
                if (shipController.get_ON()) {
                     state = GameState::PlayerTurn; 
                }
            }
        }
    }
}

void Game::update(){
    // ИСПРАВЛЕНО: Двигаем корабли за мышкой только во время фазы расстановки
    if (state == GameState::Placement) {
        shipController.update(window);
    }
}

void Game::render(){
    window.clear();

    playerBoardView.draw(window);
    enemyBoardView.draw(window);

    shipController.draw(window);

    fier_view.draw(window, playerBoardView);
    fier_view.draw(window, enemyBoardView);

    window.display();
}

void Game::render_loop() {
    window.setActive(true); 
    while (isRunning && window.isOpen()) {
        render();
    }
} 

void Game::run()
{
    renderThread = std::thread(&Game::render_loop, this);
    bool wasPressed = false;

    while (window.isOpen()&& isRunning)
    {
        handle_events();
        update();

        if (state == GameState::PlayerTurn) {
           // Срабатывает один раз в момент нажатия
            if (isPressed && !wasPressed){
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f coords = window.mapPixelToCoords(mousePos);

                sf::Vector2i targetedCell = enemyBoardView.screenToCell(coords);

                if (targetedCell.x != -1){

                    cell shotCell(targetedCell.x, targetedCell.y);

                    if (enemyBoard.can_shoot(shotCell)){
                        bool hit = enemyBoard.shoot(shotCell);
                        if (hit){
                            fier_view.addHit(shotCell);   //  огонь
                        } else {
                            fier_view.addMiss(shotCell);  //  крестик
                        }

                        if (enemyBoard.victory()) {
                            state = GameState::GameOver;
                        } else if (!hit) {
                            state = GameState::EnemyTurn;
                        }
                    }
                }
            }

            wasPressed = isPressed;
        }


        if (state == GameState::EnemyTurn) {
            // Искусственная пауза в полсекунды, чтобы робот не стрелял мгновенно
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            cell enemyShot = enemy->take_turn();
            
            bool hit = playerBoard.shoot(enemyShot); // Робот стреляет по нам
            
            if (hit) {
                // Если робот попал, передаем координату выстрела в систему отрисовки огня
                fier_view.addHit(enemyShot); 
            } else {
                fier_view.addMiss(enemyShot);  //  крестик
            }

            if (playerBoard.victory()) {
                state = GameState::GameOver;
            } else if (!hit) {
                state = GameState::PlayerTurn; // Робот промахнулся — возвращаем ход человеку
            }
        }

        // Небольшая разгрузка для процессора, чтобы бесконечный цикл логики не грузил ядро на 100%
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

    
