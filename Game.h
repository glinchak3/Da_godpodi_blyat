#include <atomic>
#include <thread>
#include "Board_view.h"
#include "Ship_controller.h"
#include "Fier_view.h"

#include "Board.h"
#include "Player.h"
#include "Ship.h"

// Состояния игры
enum class GameState {
    Placement,   // Фаза расстановки кораблей игроком
    PlayerTurn,  // Ход игрока (ожидание клика по вражескому полю)
    EnemyTurn,   // Ход компьютера
    GameOver     // Игра окончена
};

class Game
{
private:

    ShotView playerShots;
    ShotView enemyShots;

    sf::RenderWindow window;
    sf::Event event;

    BoardView playerBoardView;
    BoardView enemyBoardView;

    ship_controller shipController;

    board playerBoard;
    board enemyBoard;

    sf::Texture t4, t3, t2, t1;

    HumanPlayer* player;
    ComputerPlayer* enemy;

    std::atomic<bool> isRunning{ true }; // Флаг работы игры для синхронизации потоков
    std::atomic<GameState> state{ GameState::Placement }; // Текущая фаза игры
    std::thread renderThread;           // Поток для отрисовки графики

    void handle_events();
    void update();
    void render();
    void render_loop(); // Метод, который будет выполняться в отдельном потоке

    void init();

public:
    Game();
    ~Game();
    void run();
};