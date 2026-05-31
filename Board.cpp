#include "Board.h"

board::board() {
    grid = std::vector<std::vector<char>>(10, std::vector<char>(10, '.'));
}

void board::place_ship(ship &Ship) {
    std::vector<cell> cells = Ship.get_cells();

    for (int i = 0; i < cells.size(); i++) {
        int x = cells[i].get_x();
        int y = cells[i].get_y();

        grid[y][x] = 'S';
    }
}

bool board::shoot(cell Shoot_cell) {

    int x = Shoot_cell.get_x();
    int y = Shoot_cell.get_y();

    if (grid[y][x] == 'S') {
        grid[y][x] = 'X';
        return true;
    }

    if (grid[y][x] == '.') {
        grid[y][x] = 'O';
    }

    return false;
}

bool board::can_shoot(cell wanted_cell) const {

    int x = wanted_cell.get_x();
    int y = wanted_cell.get_y();

    return grid[y][x] == '.' || grid[y][x] == 'S';
}

bool board::victory() const {

    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {

            if (grid[y][x] == 'S') {
                return 0;
            }
        }
    }

    return 1;
}

bool board::can_place_ship(ship ship) const {
    const std::vector<cell> sh = ship.get_cells();
    for (const auto& cell : sh) {
        int curr_x = cell.get_x(); 
        int curr_y = cell.get_y();

        if ((curr_x < 0) || (curr_x >= 10) || (curr_y < 0) || (curr_y >= 10)) {
            return false;
        }

        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int check_x = curr_x + dx;
                int check_y = curr_y + dy;

                if ((check_x >= 0) && (check_x < 10) && (check_y >= 0) && (check_y < 10)) {
                    if (grid[check_y][check_x] == 'S') {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}