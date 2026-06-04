#pragma once

#include <iostream> 
#include <string> 
#include <mutex> 
#include <set> 
#include <algorithm> 
#include "Cell.h" 
#include "Board.h"

class Player {
    protected:
        board& own_board;
        board& enemy_board;
    
    public:
        Player(board& own, board& enemy) : own_board(own), enemy_board(enemy) {}
        virtual void place_ship(ship new_ship) = 0;
        virtual ~Player() = default; 
    };
    
    class HumanPlayer : public Player {
    public:
        HumanPlayer(board& own, board& enemy) : Player(own, enemy) {}
        void place_ship(ship new_ship) override {
            if (own_board.can_place_ship(new_ship)){
                own_board.place_ship(new_ship);
            }
        }
    };
    
class ComputerPlayer : public Player {
    private: 
        //смещения 
        const int dx[4] = {-1, 1, 0, 0};
        const int dy[4] = {0, 0, -1, 1};
    
        std::vector<cell> target_queue; //очередь для добивания
        std::set<std::pair<int,int>> dead_ship_zones;
    
        bool is_ship_dead(const std::vector<cell>& hits_cluster) const;
        void update_dead_ships();
    
        std::vector<cell> find_cells(char ch) const;
        void count_free_sect(int start_x, int end_x, int start_y, int end_y, int& result) const;
        cell find_sector(int x1, int x2, int y1, int y2, int depth = 0, int max_depth = 3) const;
    
    public:
        ComputerPlayer(board& own, board& enemy) : Player(own, enemy) {}
        void place_ship(ship new_ship) override;
        cell take_turn();
    };