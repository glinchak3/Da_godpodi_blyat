#include <iostream>
#include <string>
#include <mutex>
#include "Cell.h"
#include "Board.h"


class Player {
protected:
    board& own_board;
    board& enemy_board;

public:
    Player(board& own, board& enemy) : own_board(own), enemy_board(enemy) {}
    virtual void place_ship(ship new_ship) = 0;
    virtual cell take_turn() = 0;
    virtual ~Player() = default; 
};

class HumanPlayer : public Player {
public:
    HumanPlayer(board& own, board& enemy) : Player(own, enemy) {}
    void place_ship(ship new_ship) override {
        own_board.place_ship(new_ship);
    }
    cell take_turn() override {
        

    }

};

class ComputerPlayer : public Player {
private: 
    
    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};

    std::vector<cell> find_free_cells(char ch);
    void count_free_sect(int start_x, int end_x, int start_y, int end_y, int& result) const;
    cell find_sector(int x1, int x2, int y1, int y2);

public:
    ComputerPlayer(board& own, board& enemy) : Player(own, enemy) {}
    void place_ship(ship new_ship) override;
    cell take_turn() override;
};


