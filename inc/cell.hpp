#pragma once
// #include "custom_enum.hpp"

#ifndef CELL_HEADER
#define CELL_HEADER


// BEGIN_ENUM(CellType){
//     DECL_ENUM_ELEMENT(NONE),
//     DECL_ENUM_ELEMENT(SAND),
// }
// END_ENUM(CellType)
enum CellType{
    NONE,
    SAND,
    WATER,
};

struct Cell {
    CellType type;
    bool updatedThisFrame;
    // int x, y;
};
#endif