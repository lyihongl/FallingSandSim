#pragma once
#define __PIXEL_EDGE_PER_CELL 4
#define __SCREEN_WIDTH 1000
#define __SCREEN_HEIGHT 600
#define __VP_ASPECT __SCREEN_WIDTH / __SCREEN_HEIGHT
#define __SCALING_FACTOR (float)__PIXEL_EDGE_PER_CELL / __SCREEN_WIDTH
#define __PIXEL_WIDTH __SCREEN_WIDTH / __PIXEL_EDGE_PER_CELL
#define __PIXEL_HEIGHT __SCREEN_HEIGHT / __PIXEL_EDGE_PER_CELL
#include <random>

struct RNG {
    std::random_device rd;
    std::mt19937 mt;
    std::uniform_real_distribution<double> dist;
    RNG(std::mt19937 _mt, int a, int b): mt(_mt), dist(a,b) {}
    int get_num();
};

int RNG::get_num(){
    return dist(mt);
}