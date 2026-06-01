#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Board_view.h"

class ShotView {
private:
    sf::Texture hitTexture;
    sf::Texture missTexture;
    std::vector<sf::Vector2i> hits;
    std::vector<sf::Vector2i> misses;
    float hitScale = 0.0005f;
    float missScale = 0.0005f;

public:
    bool loadHit(const std::string& path);
    bool loadMiss(const std::string& path);
    void addHit(sf::Vector2i cell);
    void addMiss(sf::Vector2i cell);
    void draw(sf::RenderWindow& window, const BoardView& board);
    
    void setHitScale(float scale);
    void setMissScale(float scale);
};