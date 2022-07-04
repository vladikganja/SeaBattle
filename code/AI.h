#pragma once
#include "Field.h"
#include <random>
#include <chrono>
#include <exception>

class AI {
private:
    static bool random;
    static int last_line;
    static int last_column;

public:
    static void make_turn(std::vector<std::vector<std::shared_ptr<Tile>>>& field);
    static void reset();
};
