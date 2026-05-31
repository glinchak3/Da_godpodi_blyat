#pragma once

#include <fstream>
#include <iostream>
#include <mutex>
#include <vector>
#include "Cell.h"


class ship{
    private:
        int size;
        std::vector<cell> cells;
        std::vector<bool> hit_cells;
    public:
        ship(int Size, std::vector<cell> Cells, std::vector<bool> Hit_cells);
        ship(int Size, std::vector<cell> Cells);
        ship(std::vector<sf::Vector2i> cells);
        void mark_hit_cell(cell Hit_cell);
        bool is_dead();
        std::vector<cell> get_hit_cells();
        int get_size();
        std::vector<cell> get_cells();
};