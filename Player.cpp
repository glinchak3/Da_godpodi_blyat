#include <vector>
#include <thread>
#include <cstdlib>
#include <set>
#include <algorithm>
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

cell ComputerPlayer::find_sector(int x1, int x2, int y1, int y2, int depth, int max_depth) {
    // Базовый случай: одна клетка
    if (x1 == x2 && y1 == y2) {
        return enemy_board.can_shoot(cell(x1, y1)) ? cell(x1, y1) : cell(-1, -1);
    }
    
    // Ограничение глубины рекурсии
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
    
    // Деление сектора
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
    
    // Считаем без потоков для маленьких секторов
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
            // Случайный выбор с fallback
            if (std::rand() % 2 == 0) {
                cell res = find_sector(x1, mid_x1, y1, mid_y1, depth + 1, max_depth);
                if (res.get_x() != -1) return res;
                return find_sector(start_x2, x2, start_y2, y2, depth + 1, max_depth);
            } else {
                cell res = find_sector(start_x2, x2, start_y2, y2, depth + 1, max_depth);
                if (res.get_x() != -1) return res;
                return find_sector(x1, mid_x1, y1, mid_y1, depth + 1, max_depth);
            }
        }
    }
    
    if (count1 > 0) return find_sector(x1, mid_x1, y1, mid_y1, depth + 1, max_depth);
    if (count2 > 0) return find_sector(start_x2, x2, start_y2, y2, depth + 1, max_depth);
    
    return cell(-1, -1);
}
    
void ComputerPlayer::place_ship(ship new_ship) {
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

// ComputerPlayer.cpp

// Новый метод: проверяет, потоплен ли корабль
bool ComputerPlayer::is_ship_sunk(const std::vector<cell>& hits_cluster) {
    if (hits_cluster.empty()) return false;
    
    // Находим границы
    int min_x = 10, max_x = -1, min_y = 10, max_y = -1;
    for (const auto& h : hits_cluster) {
        min_x = std::min(min_x, h.get_x());
        max_x = std::max(max_x, h.get_x());
        min_y = std::min(min_y, h.get_y());
        max_y = std::max(max_y, h.get_y());
    }
    
    // Проверяем, что все клетки в прямоугольнике - это попадания (нет дырок)
    for (int x = min_x; x <= max_x; ++x) {
        for (int y = min_y; y <= max_y; ++y) {
            bool found = false;
            for (const auto& h : hits_cluster) {
                if (h.get_x() == x && h.get_y() == y) {
                    found = true;
                    break;
                }
            }
            if (!found) return false; // Есть дырка - не потоплен
        }
    }
    
    // Проверяем, есть ли живые клетки ВОКРУГ корабля (не по диагоналям)
    if (min_x == max_x) { // Горизонтальный
        int row = min_x;
        // Слева и справа от концов
        if (min_y > 0 && enemy_board.can_shoot(cell(row, min_y - 1))) return false;
        if (max_y < 9 && enemy_board.can_shoot(cell(row, max_y + 1))) return false;
        // Сверху и снизу от каждой палубы
        for (int y = min_y; y <= max_y; ++y) {
            if (row > 0 && enemy_board.can_shoot(cell(row - 1, y))) return false;
            if (row < 9 && enemy_board.can_shoot(cell(row + 1, y))) return false;
        }
        return true;
    }
    else if (min_y == max_y) { // Вертикальный
        int col = min_y;
        // Сверху и снизу от концов
        if (min_x > 0 && enemy_board.can_shoot(cell(min_x - 1, col))) return false;
        if (max_x < 9 && enemy_board.can_shoot(cell(max_x + 1, col))) return false;
        // Слева и справа от каждой палубы
        for (int x = min_x; x <= max_x; ++x) {
            if (col > 0 && enemy_board.can_shoot(cell(x, col - 1))) return false;
            if (col < 9 && enemy_board.can_shoot(cell(x, col + 1))) return false;
        }
        return true;
    }
    
    return false;
}
// Новый метод: обновляет зоны потопленных кораблей
void ComputerPlayer::update_sunk_ships() {
    std::vector<cell> hits = find_free_cells('X');

    sunk_ship_zones.clear();

    if (hits.empty()) return;

    std::vector<std::vector<cell>> clusters;
    std::vector<bool> used(hits.size(), false);

    for (size_t i = 0; i < hits.size(); ++i) {
        if (used[i]) continue;

        std::vector<cell> cluster;
        cluster.push_back(hits[i]);
        used[i] = true;

        bool added = true;

        while (added) {
            added = false;

            for (size_t j = 0; j < hits.size(); ++j) {
                if (used[j]) continue;

                for (const auto& c : cluster) {
                    if (abs(hits[j].get_x() - c.get_x()) +
                        abs(hits[j].get_y() - c.get_y()) == 1) {

                        cluster.push_back(hits[j]);
                        used[j] = true;
                        added = true;
                        break;
                    }
                }
            }
        }

        clusters.push_back(cluster);
    }

    for (const auto& cluster : clusters) {
        if (!is_ship_sunk(cluster)) continue;

        int min_x = 10, max_x = -1, min_y = 10, max_y = -1;

        for (const auto& h : cluster) {
            min_x = std::min(min_x, h.get_x());
            max_x = std::max(max_x, h.get_x());
            min_y = std::min(min_y, h.get_y());
            max_y = std::max(max_y, h.get_y());
        }

        for (int x = min_x - 1; x <= max_x + 1; ++x)
            for (int y = min_y - 1; y <= max_y + 1; ++y)
                if (x >= 0 && x < 10 && y >= 0 && y < 10)
                    sunk_ship_zones.insert({x, y});
    }
}

cell ComputerPlayer::take_turn() {
    update_sunk_ships();

    // 1. СНАЧАЛА ПРОВЕРЯЕМ ОЧЕРЕДЬ
    while (!target_queue.empty()) {
        cell next = target_queue.back();
        target_queue.pop_back();
        
        if (sunk_ship_zones.count({next.get_x(), next.get_y()})) {
            continue;
        }
        
        if (enemy_board.can_shoot(next)) {
            return next;
        }
    }

    // 2. АНАЛИЗИРУЕМ ПОПАДАНИЯ
    std::vector<cell> hits = find_free_cells('X');
    
    if (!hits.empty()) {
        target_queue.clear();
        
        // Группируем попадания в кластеры
        std::vector<std::vector<cell>> clusters;
        std::vector<bool> used(hits.size(), false);
        
        for (size_t i = 0; i < hits.size(); ++i) {
            if (used[i]) continue;
            
            std::vector<cell> cluster;
            cluster.push_back(hits[i]);
            used[i] = true;
            
            bool added = true;
            while (added) {
                added = false;
                for (size_t j = 0; j < hits.size(); ++j) {
                    if (used[j]) continue;
                    
                    for (const auto& c : cluster) {
                        // Соседство по горизонтали или вертикали
                        if ((abs(hits[j].get_x() - c.get_x()) == 1 && hits[j].get_y() == c.get_y()) ||
                            (abs(hits[j].get_y() - c.get_y()) == 1 && hits[j].get_x() == c.get_x())) {
                            cluster.push_back(hits[j]);
                            used[j] = true;
                            added = true;
                            break;
                        }
                    }
                }
            }
            clusters.push_back(cluster);
        }
        
        // Обрабатываем каждый кластер
        for (const auto& cluster : clusters) {
            if (is_ship_sunk(cluster)) {
                continue; // Потоплен - пропускаем
            }
            
            // Находим границы
            int min_x = 10, max_x = -1, min_y = 10, max_y = -1;
            for (const auto& h : cluster) {
                min_x = std::min(min_x, h.get_x());
                max_x = std::max(max_x, h.get_x());
                min_y = std::min(min_y, h.get_y());
                max_y = std::max(max_y, h.get_y());
            }
            
            // ОДНО попадание - добавляем ВСЕ 4 стороны
            if (cluster.size() == 1) {
                for (int i = 0; i < 4; ++i) {
                    int nx = min_x + dx[i];
                    int ny = min_y + dy[i];
                    if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10) {
                        cell next(nx, ny);
                        if (!sunk_ship_zones.count({nx, ny}) && enemy_board.can_shoot(next)) {
                            target_queue.push_back(next);
                        }
                    }
                }
            }
            // ГОРИЗОНТАЛЬНЫЙ корабль
            else if (min_x == max_x) {
                if (min_y > 0) {
                    cell left(min_x, min_y - 1);
                    if (!sunk_ship_zones.count({left.get_x(), left.get_y()}) && enemy_board.can_shoot(left)) {
                        target_queue.push_back(left);
                    }
                }
                if (max_y < 9) {
                    cell right(min_x, max_y + 1);
                    if (!sunk_ship_zones.count({right.get_x(), right.get_y()}) && enemy_board.can_shoot(right)) {
                        target_queue.push_back(right);
                    }
                }
            }
            // ВЕРТИКАЛЬНЫЙ корабль
            else if (min_y == max_y) {
                if (min_x > 0) {
                    cell up(min_x - 1, min_y);
                    if (!sunk_ship_zones.count({up.get_x(), up.get_y()}) && enemy_board.can_shoot(up)) {
                        target_queue.push_back(up);
                    }
                }
                if (max_x < 9) {
                    cell down(max_x + 1, min_y);
                    if (!sunk_ship_zones.count({down.get_x(), down.get_y()}) && enemy_board.can_shoot(down)) {
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

    // 3. НЕТ ПОПАДАНИЙ - ищем новый сектор
    cell target = find_sector(0, 9, 0, 9, 0, 3);
    if (target.get_x() != -1 && !sunk_ship_zones.count({target.get_x(), target.get_y()})) {
        return target;
    }
    
    // 4. Запасной вариант
    while (true) {
        int r = rand() % 10;
        int c = rand() % 10;
        if (!sunk_ship_zones.count({r, c}) && enemy_board.can_shoot(cell(r, c))) {
            return cell(r, c);
        }
    }
}