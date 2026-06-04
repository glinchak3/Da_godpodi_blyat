#pragma once

#include <iostream>
#include <SFML/System/Vector2.hpp>



class cell{
    private:
        int x;
        int y;

    public:
        cell():x(0),y(0){}
        cell(int x, int y): x(x), y(y){}
        int get_x() const{
            return x;
        };
        int get_y() const{
            return y;
        };
        sf::Vector2i to_sf() const {
            return sf::Vector2i(x, y);
        }
};
