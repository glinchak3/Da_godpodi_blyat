#include <fstream>
#include <iostream>
#include <mutex>
#pragma once


class cell{
    private:
        int x;
        int y;

    public:
        cell(int x, int y): x(x), y(y){}
        int get_x() const{
            return x;
        };
        int get_y() const{
            return y;
        };
};
