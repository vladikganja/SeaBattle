#pragma once
#include "Field.h"
#include <random>
#include <chrono>

class ShipsGenerator {
private:

public:
    static void place_ships(std::vector<std::vector<std::shared_ptr<Tile>>>& field, 
                            std::vector<std::shared_ptr<Tile>>& ships, bool hide_ships);
};
