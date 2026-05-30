#pragma once

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
        return cell(0, 0);
    }

};

