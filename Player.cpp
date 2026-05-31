#include <vector>
#include <thread>
#include <cstdlib>
#include <functional>
#include "Player.h"





//Игрок-компьютер
std::vector<cell> ComputerPlayer::find_free_cells(char ch) {
    std::vector<cell> cells;
    for (int r = 0; r < 10; ++r) {
        for (int c = 0; c < 10; ++c) {
            if (enemy_board.cell_state(cell(r, c)) == ch) {
                cells.push_back(cell(r, c));
            }
        }
    }
    return cells;
}

void ComputerPlayer::count_free_sect(int start_x, int end_x, int start_y, int end_y, int& result) const {
    result = 0;
    for (int x = start_x; x <= end_x; x++) {
        for (int y = start_y; y <= end_y; y++) {
            if (x >= 0 && x < 10 && y >= 0 && y < 10) {
                if (enemy_board.can_shoot(cell(x, y))) {
                    result++;
                }
            }
        }
    }
}

cell ComputerPlayer::find_sector(int x1, int x2, int y1, int y2, int depth = 0, int max_depth = 3) {

    if (depth >= max_depth) {
        int count1 = 0, count2 = 0;
        count_free_sect(x1, x2, y1, y2, count1);
        // В зависимости от результатов, выбрать сторону
        if (count1 > 0) {
            return cell(x1, y1); // или другая логика
        } else {
            return cell(-1, -1);
        }
    }
    
    if (x1 == x2 && y1 == y2) {
        return enemy_board.can_shoot(cell(x1, y1)) ? cell(x1, y1) : cell(-1, -1);
    } 
        
    int mid_x1 = x2;
    int start_x2 = x1;
    int mid_y1 = y2;
    int start_y2 = y1;

    if ((x2 - x1) >= (y2 - y1)) {
        mid_x1 = x1 + (x2 - x1) / 2;
        start_x2 = mid_x1 + 1;
    } else {
        mid_y1 = y1 + (y2 - y1) / 2;
        start_y2 = mid_y1 + 1;
    }

    int count1 = 0, count2 = 0;
        
    std::thread t1(&ComputerPlayer::count_free_sect, this, x1, mid_x1, y1, mid_y1, std::ref(count1));
    std::thread t2(&ComputerPlayer::count_free_sect, this, start_x2, x2, start_y2, y2, std::ref(count2));
        
    t1.join(); 
    t2.join(); 

    if ((count1 >= count2) && (count1 > 0)) {
        return find_sector(x1, mid_x1, y1, mid_y1, depth + 1, max_depth);
    } else if (count2 > 0) {
        return find_sector(start_x2, x2, start_y2, y2, depth + 1, max_depth);
    }
    return cell(-1, -1);
}

void ComputerPlayer::place_ship(ship new_ship) override {
    std::vector<int> fleet = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
        int board_size = 10;

    for (int ship_len : fleet) {
        bool placed = false;
        while (!placed) {
            int r = std::rand() % board_size;
            int c = std::rand() % board_size;
            int dir = (std::rand() % 2 == 0) ? 1 : 0;

            std::vector<cell> cells_to_place;
            for (int i = 0; i < ship_len; ++i) {
                int curr_row = r + (dir == 1 ? i : 0);
                int curr_col = c + (dir == 0 ? i : 0);
                cells_to_place.push_back(cell(curr_row, curr_col)); 
            }

            ship temp(ship_len, cells_to_place);

            if (own_board.can_place_ship(temp)) {
                own_board.place_ship(temp);
                placed = true;
            }
        }
    }
}

cell ComputerPlayer::take_turn() override {

    std::vector<cell> hits = find_free_cells('X'); 
        
    if (!hits.empty()) {

        bool line_detected = hits.size() >= 2;
        bool is_horizontal = line_detected && (hits[0].get_x() == hits[1].get_x());
            
        for (const auto& hit : hits) {
            for (int i = 0; i < 4; ++i) {
            
                if (line_detected && is_horizontal && dx[i] != 0) continue;
                if (line_detected && !is_horizontal && dy[i] != 0) continue;

                int next_x = hit.get_x() + dx[i];
                int next_y = hit.get_y() + dy[i];

                if (next_x >= 0 && next_x < 10 && next_y >= 0 && next_y < 10) {
                    cell next_cell(next_x, next_y);
                    if (enemy_board.can_shoot(next_cell)) {
                        return next_cell; 
                    }
                }
            }
        }
    }

    cell target = find_sector(0, 9, 0, 9);
        
    if (target.get_x() != -1) {
        return target;
    }

    // Запасной вариант: стреляем в первую попавшуюся доступную клетку
    for (int r = 0; r < 10; ++r) {
        for (int c = 0; c < 10; ++c) {
            cell fallback_cell(r, c);
            if (enemy_board.can_shoot(fallback_cell)) {
                return fallback_cell;
            }
        }
    }
    return cell(0, 0);
}

