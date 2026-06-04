#include "Fier_view.h"

bool ShotView::load_hit(const std::string& path){
    return hit_texture.loadFromFile(path);  // hitTexture → hit_texture
}

bool ShotView::load_miss(const std::string& path){
    return miss_texture.loadFromFile(path);  // missTexture → miss_texture
}

void ShotView::add_hit(sf::Vector2i cell){
    hits.push_back(cell);
}

void ShotView::add_miss(sf::Vector2i cell){
    misses.push_back(cell);
}

void ShotView::draw(sf::RenderWindow& window, const board_view& board){  // BoardView → board_view
    sf::FloatRect bounds = board.get_bounds();
    float cellW = bounds.width / 11.f;
    float cellH = bounds.height / 11.f;
    
    for (auto& h : hits){
        sf::Sprite s;
        s.setTexture(hit_texture);  // hitTexture → hit_texture
        s.setScale(hit_scale, hit_scale);
        sf::Vector2f cell_pos = board.cell_to_screen(h.x, h.y);
        s.setPosition(cell_pos.x + 4.f, cell_pos.y + 2.f);
        window.draw(s);
    }

    for (auto& m : misses){
        sf::Sprite s;
        s.setTexture(miss_texture);  // missTexture → miss_texture
        s.setScale(miss_scale, miss_scale);
        sf::Vector2f cell_pos = board.cell_to_screen(m.x, m.y);
        s.setPosition(cell_pos.x + 1.f, cell_pos.y + 1.f);
        window.draw(s);
    }
}

void ShotView::set_hit_scale(float scale) {
    hit_scale = scale;
}

void ShotView::set_miss_scale(float scale) {  
    miss_scale = scale;
}