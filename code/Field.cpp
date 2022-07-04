#include "Field.h"

sf::Color Tile::BLUEBG = sf::Color(89, 236, 255);
sf::Color Tile::BLUE = sf::Color(49, 140, 231);
sf::Color Tile::DARKBLUE = sf::Color(0, 56, 154);
sf::Color Tile::GRAY = sf::Color(100, 100, 100);
sf::Color Tile::ORANGE = sf::Color(240, 116, 39);
sf::Color Tile::RED = sf::Color(235, 54, 41);

float Tile::tile_size = 32.f;
int Tile::__id = 0;

Tile::Tile() {
    status = Status::VOID;
    tile_bg.setFillColor(BLUEBG);
    tile.setFillColor(BLUE);
    dir = 0;
    len = 0;
    id = __id;
    __id++;
}

void Tile::change_status(Status _status, bool hide_ship) {
    status = _status;
    switch (status) {
    case Status::MISS:
        color = DARKBLUE;
        break;
    case Status::HURT:
        color = ORANGE;
        break;
    case Status::DEAD:
        color = RED;
        break;
    case Status::SHIP:
        if (hide_ship)
            color = BLUE;
        else
            color = GRAY;
        break;
    case Status::VOID:
        color = BLUE;
        break;
    default:
        break;
    }
    tile.setFillColor(color);
}

void Tile::show(sf::RenderWindow& window) {
    window.draw(tile_bg);
    window.draw(tile);
}

sf::FloatRect Tile::get_coord() {
    return tile_bg.getGlobalBounds();
}

Status Tile::get_status() {
    return status;
}

void Tile::set_pos(sf::Vector2f pos) {
    tile_bg.setPosition(pos);
    tile.setPosition({ pos.x + 2.f, pos.y + 2.f });
}

void Tile::set_proper_size() {
    tile_bg.setSize({tile_size, tile_size});
    tile.setSize({ tile_size - 4.f, tile_size - 4.f});
}

void Tile::set_dir(int _dir) {
    dir = _dir;
}

void Tile::set_len(int _len) {
    len = _len;
}

int Tile::get_dir() {
    return dir;
}

int Tile::get_len() {
    return len;
}

int Tile::get_id() {
    return id;
}

// FIELD //////////////////////////////////////////////////////////////////////////////////////////

void Field::show(sf::RenderWindow& window) {
    window.draw(outline);
    for (size_t i = 0; i < field.size(); i++) {
        for (size_t j = 0; j < field[i].size(); j++) {
            field[i][j]->show(window);
        }
    }
}

Field::Field() : field_size(0) {
    outline.setFillColor(Tile::BLUEBG);
    outline.setPosition(0.f, 0.f);
    outline.setSize({static_cast<float>(WINDOW_HEIGHT), static_cast<float>(WINDOW_HEIGHT)});
}

Field::~Field() {
    Tile::__id = 0;
}

sf::FloatRect Field::get_coord() {
    return outline.getLocalBounds();
}

std::vector<std::vector<std::shared_ptr<Tile>>>& Field::get_field() {
    return field;
}

std::vector<std::shared_ptr<Tile>>& Field::get_ships() {
    return ships;
}

void Field::reset() {
    for (size_t i = 0; i < field_size; i++) {
        for (size_t j = 0; j < field_size; j++) {
            field[i][j]->change_status(Status::VOID, false);
        }
    }
    ships.clear();
}

void Field::generate(size_t size, bool side) {
    field_size = size;

    float left_offs, top_offs;

    if (side) {
        left_offs = static_cast<float>(WINDOW_WIDTH) - static_cast<float>(WINDOW_HEIGHT) + 10.f;
        outline.setPosition({ left_offs - 10.f, 0.f });
    }
    else {
        left_offs = 10.f;
    }

    top_offs = 10.f;

    Tile::tile_size = (WINDOW_HEIGHT - 20.f) / size;

    field.resize(field_size);
    for (size_t i = 0; i < field_size; i++) {
        field[i].resize(field_size);
        for (size_t j = 0; j < field_size; j++) {
            field[i][j] = std::shared_ptr<Tile>(new Tile);
            field[i][j]->set_pos({ j * Tile::tile_size + left_offs, i * Tile::tile_size + top_offs});
            field[i][j]->set_proper_size();
        }
    }
}
