#pragma once
#include "SFML/Graphics.hpp"
#include "Utils.h"
#include <vector>

enum class Status {
    VOID,
    MISS,
    SHIP,
    HURT,
    DEAD
};

class Tile {
private:
    sf::RectangleShape tile;
    sf::RectangleShape tile_bg;
    sf::Color color;
    sf::Color color_bg;

    Status status;
    int dir;
    int len;
    int id;

public:

    Tile();
    static int __id;
    static sf::Color BLUEBG;    // bg
    static sf::Color BLUE;      // water
    static sf::Color DARKBLUE;  // miss
    static sf::Color GRAY;      // unbroken ship
    static sf::Color ORANGE;    // hurt ship 
    static sf::Color RED;       // dead ship

    static float tile_size;

    Status get_status();
    void set_pos(sf::Vector2f pos);
    void set_proper_size();
    void set_dir(int dir);
    void set_len(int len);
    int get_dir();
    int get_len();
    int get_id();
    void change_status(Status status, bool hide_ship = false);
    void show(sf::RenderWindow& window);

    sf::FloatRect get_coord();
};

class Field {
private:
    size_t field_size;
    std::vector<std::vector<std::shared_ptr<Tile>>> field;
    std::vector<std::shared_ptr<Tile>> ships;
    sf::RectangleShape outline;

public:
    Field();
    ~Field();

    std::vector<std::vector<std::shared_ptr<Tile>>>& get_field();
    std::vector<std::shared_ptr<Tile>>& get_ships();
    void reset();
    void generate(size_t size, bool side);
    sf::FloatRect get_coord();
    void show(sf::RenderWindow& window);
};
