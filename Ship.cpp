
#include "Ship.h"

ship::ship(int Size, std::vector<cell> Cells, std::vector<bool> Hit_cells) {
    size = Size;
    cells = Cells;
    hit_cells = Hit_cells;
}

ship::ship(int Size, std::vector<cell> Cells) {
    size = Size;
    cells = Cells;
    hit_cells = std::vector<bool>(size,0);
}

ship::ship(std::vector<sf::Vector2i> cs){
    size = cs.size();
    std::vector<cell> Cells;
    for(int i = 0; i<size;i++){
        Cells.push_back(cell(cs[i].x,cs[i].y));
    }
    cells = Cells;
    hit_cells = std::vector<bool>(size,0);
}

void ship::mark_hit_cell(cell Hit_cell) {

    for (int i = 0; i < cells.size(); i++) {

        if (cells[i].get_x() == Hit_cell.get_x() && cells[i].get_y() == Hit_cell.get_y()) {
            hit_cells[i] = true;
        }
    }
};

bool ship::is_dead() {
    for (int i = 0; i < hit_cells.size(); i++) {
        if (hit_cells[i] == false) {
            return false;
        }
    }
    return true;
}

std::vector<cell> ship::get_hit_cells() const {
    std::vector<cell> damaged_cells;
    for (size_t i = 0; i < cells.size(); i++) {
        if (hit_cells[i]) {
            damaged_cells.push_back(cells[i]);
        }
    }
    return damaged_cells;
}


int ship::get_size() {
    return size;
}

std::vector<cell> ship::get_cells() {
    return cells;
}
