#include <vector>
#include <thread>
#include <cstdlib>
#include <set>
#include <algorithm>
#include <functional>
#include "Player.h"

//Игрок-компьютер

//собирает клетки с определенным состоянием: например "X"
std::vector<cell> ComputerPlayer::find_cells(char ch) const {
    std::vector<cell> cells;
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            if (enemy_board.cell_state(cell(x, y)) == ch) {
                cells.push_back(cell(x, y));
            }
        }
    }
    return cells;
}

// считает клетки куда можно стрелять для сектора
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

// ищет сектор где больше всего клеток  
cell ComputerPlayer::find_sector(int x1, int x2, int y1, int y2, int depth, int max_depth) const{
    if (x1 == x2 && y1 == y2) {
        return enemy_board.can_shoot(cell(x1, y1)) ? cell(x1, y1) : cell(-1, -1);
    }
    
    // ограничение глубины рекурсии
    if (depth >= max_depth) {
        for (int x = x1; x <= x2; x++) {
            for (int y = y1; y <= y2; y++) {
                if (x >= 0 && x < 10 && y >= 0 && y < 10) {
                    if (enemy_board.can_shoot(cell(x, y))) {
                        return cell(x, y);
                    }
                }
            }
        }
        return cell(-1, -1);
    }
    
    // делим сектор
    int mid_x1 = x2, start_x2 = x1;
    int mid_y1 = y2, start_y2 = y1;
    
    if ((x2 - x1) >= (y2 - y1)) {
        mid_x1 = x1 + (x2 - x1) / 2;
        start_x2 = mid_x1 + 1;
    } else {
        mid_y1 = y1 + (y2 - y1) / 2;
        start_y2 = mid_y1 + 1;
    }
    
    int count1 = 0, count2 = 0;
    
    // считаем без потоков для маленьких секторов
    if ((x2-x1+1)*(y2-y1+1) < 16) {
        count_free_sect(x1, mid_x1, y1, mid_y1, count1);
        count_free_sect(start_x2, x2, start_y2, y2, count2);
    } else {
        std::thread t1(&ComputerPlayer::count_free_sect, this, x1, mid_x1, y1, mid_y1, std::ref(count1));
        std::thread t2(&ComputerPlayer::count_free_sect, this, start_x2, x2, start_y2, y2, std::ref(count2));
        t1.join();
        t2.join();
    }
    
    if (count1 > 0 && count2 > 0) {
        if (count1 > count2) {
            return find_sector(x1, mid_x1, y1, mid_y1, depth + 1, max_depth);
        } else if (count2 > count1) {
            return find_sector(start_x2, x2, start_y2, y2, depth + 1, max_depth);
        } else {
            //из одинаковых выбираем случайно
            if (std::rand() % 2 == 0) {
                cell res = find_sector(x1, mid_x1, y1, mid_y1, depth + 1, max_depth);
                if (res.get_x() != -1){
                    return res;
                }
                return find_sector(start_x2, x2, start_y2, y2, depth + 1, max_depth);
            } else {
                cell res = find_sector(start_x2, x2, start_y2, y2, depth + 1, max_depth);
                if (res.get_x() != -1){
                    return res;
                } 
                return find_sector(x1, mid_x1, y1, mid_y1, depth + 1, max_depth);
            }
        }
    } else if (count1 > 0){
        return find_sector(x1, mid_x1, y1, mid_y1, depth + 1, max_depth);
    } else if (count2 > 0){
        return find_sector(start_x2, x2, start_y2, y2, depth + 1, max_depth);
    } 
    return cell(-1, -1);
}
  
//расставляет корабли
void ComputerPlayer::place_ship(ship new_ship) {
    std::vector<int> fleet = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    int board_size = 10;

    for (int ship_len : fleet) {
        bool placed = false;
        while (!placed) {
            int x = std::rand() % board_size;
            int y = std::rand() % board_size;
            //int dir = (std::rand() % 2 == 0) ? 1 : 0;

            std::vector<cell> cells_to_place;
            for (int i = 0; i < ship_len; i++) {
                int curr_x = x + i; //(dir == 1 ? i : 0);
                int curr_y = y; //+ (dir == 0 ? i : 0);
                cells_to_place.push_back(cell(curr_x, curr_y)); 
            }

            ship tmp(ship_len, cells_to_place);

            if (own_board.can_place_ship(tmp)) {
                own_board.place_ship(tmp);
                placed = true;
            }
        }
    }
}

//проверяет потоплен ли корабль
bool ComputerPlayer::is_ship_dead(const std::vector<cell>& hits_c) const {
    if (hits_c.empty()) {
        return false;
    }
    
    //локализуем корабль
    int min_x = 10, max_x = -1, min_y = 10, max_y = -1;
    for (const auto& h : hits_c) {
        min_x = std::min(min_x, h.get_x());
        max_x = std::max(max_x, h.get_x());
        min_y = std::min(min_y, h.get_y());
        max_y = std::max(max_y, h.get_y());
    }
    
    // проверяем что нет дырок
    for (int x = min_x; x <= max_x; x++) {
        for (int y = min_y; y <= max_y; y++) {
            bool found = std::any_of(hits_c.begin(), hits_c.end(),[x, y](const cell& h) {
                return h.get_x() == x && h.get_y() == y;
            });
        if (!found) return false; 
        }
    }
    
    // Проверяем есть ли живые клетки вокруг корабля
    if (min_x == max_x) { //горизонтальный
        int row = min_x;
        if (min_y > 0 && enemy_board.can_shoot(cell(row, min_y - 1))) {
            return false;
        }
        if (max_y < 9 && enemy_board.can_shoot(cell(row, max_y + 1))){
            return false;
        } 
        for (int y = min_y; y <= max_y; y++) {
            if (row > 0 && enemy_board.can_shoot(cell(row - 1, y))){
                return false;
            } 
            if (row < 9 && enemy_board.can_shoot(cell(row + 1, y))){
                return false;
            } 
        }
        return true;

    } else if (min_y == max_y) { // вертикальный
        int col = min_y;
        if (min_x > 0 && enemy_board.can_shoot(cell(min_x - 1, col))){
            return false;
        } 
        if (max_x < 9 && enemy_board.can_shoot(cell(max_x + 1, col))){
            return false;
        } 
        for (int x = min_x; x <= max_x; x++) {
            if (col > 0 && enemy_board.can_shoot(cell(x, col - 1))){
                return false;
            } 
            if (col < 9 && enemy_board.can_shoot(cell(x, col + 1))){
                return false;
            } 
        }
        return true;
    }
    return false;
}

//обновляет зоны вокруг потопленных кораблей
void ComputerPlayer::update_dead_ships() {

    std::vector<cell> hits = find_cells('X');
    dead_ship_zones.clear();
    if (hits.empty()) return;

    std::vector<std::vector<cell>> clasters;
    std::vector<bool> used(hits.size(), false);

    // группируем попадания (соседние по вертикали/горизонтали)
    for (int i = 0; i < hits.size(); i++) {
        if (used[i]) continue;
        std::vector<cell> claster;
        claster.push_back(hits[i]);
        used[i] = true;
        
        bool added = true;
        while (added) {
            added = false;
            for (int j = 0; j < hits.size(); j++) {
                if (used[j]) continue;
                auto is_adjacent = [&](const cell& c) {
                    return std::abs(hits[j].get_x() - c.get_x()) + std::abs(hits[j].get_y() - c.get_y()) == 1;
                };
                bool found = std::any_of(claster.begin(), claster.end(), is_adjacent);
                if (found) {
                    claster.push_back(hits[j]);
                    used[j] = true;
                    added = true;
                }
            }
        }
        clasters.push_back(claster);
    }

    // Помечаем зоны вокруг мёртвых кораблей
    for (const auto& claster : clasters) {
        if (!is_ship_dead(claster)) continue;

        int min_x = 10, max_x = -1, min_y = 10, max_y = -1;

        std::for_each(claster.begin(), claster.end(),
            [&](const cell& h) {
                min_x = std::min(min_x, h.get_x());
                max_x = std::max(max_x, h.get_x());
                min_y = std::min(min_y, h.get_y());
                max_y = std::max(max_y, h.get_y());
            });

        for (int x = min_x - 1; x <= max_x + 1; x++){
            for (int y = min_y - 1; y <= max_y + 1; y++){
                if (x >= 0 && x < 10 && y >= 0 && y < 10){
                    dead_ship_zones.insert({x, y});
                }
            }
        }       
    }
}

cell ComputerPlayer::take_turn() {

    update_dead_ships();
    //проверяем были ли незакрытые попадания
    while (!target_queue.empty()) {
        cell next = target_queue.back();
        target_queue.pop_back();
        if (dead_ship_zones.count({next.get_x(), next.get_y()})) {
            continue;
        }
        if (enemy_board.can_shoot(next)) {
            return next;
        }
    }

    std::vector<cell> hits = find_cells('X');
    if (!hits.empty()) {
        target_queue.clear();
        
        std::vector<std::vector<cell>> sects;
        std::vector<bool> used(hits.size(), false);
        
        for (int i = 0; i < hits.size(); i++) {
            if (used[i]) continue;
            
            std::vector<cell> sect;
            sect.push_back(hits[i]);
            used[i] = true;
            
            bool added = true;
            while (added) {
                added = false;
                for (int j = 0; j < hits.size(); j++) {
                    if (used[j]) continue;
                    
                    for (const auto& c : sect) {
                        auto is_adjacent_to = [&](const cell& c) {
                            return (abs(hits[j].get_x() - c.get_x()) == 1 && hits[j].get_y() == c.get_y()) || (abs(hits[j].get_y() - c.get_y()) == 1 && hits[j].get_x() == c.get_x());
                        };
                        if (std::any_of(sect.begin(), sect.end(), is_adjacent_to)) {
                            sect.push_back(hits[j]);
                            used[j] = true;
                            added = true;
                        }
                    }
                }
            }
            sects.push_back(sect);
        }
        
        // Обрабатываем каждый участок
        for (const auto& sect : sects) {
            if (is_ship_dead(sect)) {
                continue;
            }
            
            int min_x = 10, max_x = -1, min_y = 10, max_y = -1;
            for (const auto& h : sect) {
                min_x = std::min(min_x, h.get_x());
                max_x = std::max(max_x, h.get_x());
                min_y = std::min(min_y, h.get_y());
                max_y = std::max(max_y, h.get_y());
            }
            
    
            if (sect.size() == 1) {
                for (int i = 0; i < 4; i++) {
                    int nx = min_x + dx[i];
                    int ny = min_y + dy[i];
                    if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10) {
                        cell next(nx, ny);
                        if (!dead_ship_zones.count({nx, ny}) && enemy_board.can_shoot(next)) {
                            target_queue.push_back(next);
                        }
                    }
                }
            } else if (min_x == max_x) {
                if (min_y > 0) {
                    cell left(min_x, min_y - 1);
                    if (!dead_ship_zones.count({left.get_x(), left.get_y()}) && enemy_board.can_shoot(left)) {
                        target_queue.push_back(left);
                    }
                }
                if (max_y < 9) {
                    cell right(min_x, max_y + 1);
                    if (!dead_ship_zones.count({right.get_x(), right.get_y()}) && enemy_board.can_shoot(right)) {
                        target_queue.push_back(right);
                    }
                }
            } else if (min_y == max_y) {
                if (min_x > 0) {
                    cell up(min_x - 1, min_y);
                    if (!dead_ship_zones.count({up.get_x(), up.get_y()}) && enemy_board.can_shoot(up)) {
                        target_queue.push_back(up);
                    }
                }
                if (max_x < 9) {
                    cell down(max_x + 1, min_y);
                    if (!dead_ship_zones.count({down.get_x(), down.get_y()}) && enemy_board.can_shoot(down)) {
                        target_queue.push_back(down);
                    }
                }
            }
        }
        
        if (!target_queue.empty()) {
            cell next = target_queue.back();
            target_queue.pop_back();
            return next;
        }
    }

    //ищем новый сектор
    cell target = find_sector(0, 9, 0, 9, 0, 3);
    if (target.get_x() != -1 && !dead_ship_zones.count({target.get_x(), target.get_y()})) {
        return target;
    }
    
    // запасной вариант
    while (true) {
        int x = rand() % 10;
        int y = rand() % 10;
        if (!dead_ship_zones.count({x, y}) && enemy_board.can_shoot(cell(x, y))) {
            return cell(x, y);
        }
    }
}