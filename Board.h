#include <fstream>
#include <iostream>
#include <mutex>
#include <vector>
#include "Cell.h"
#include "Ship.h"


class board{
    private:
        int size = 100;
        std::vector<std::vector<char>> grid;
        mutable std::mutex mtx;
    public:
        board(); 
        void place_ship(ship & Ship);
        bool shoot(cell Shoot_cell);
        bool can_shoot(cell wanted_cell) const;
        bool victory() const;
        bool can_place_ship(ship ship) const;
        char cell_state(cell s) const {
            int x = s.get_x();
            int y = s.get_y();
            return grid[y][x];
        }
};


