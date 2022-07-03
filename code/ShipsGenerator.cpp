#include "ShipsGenerator.h"

constexpr int LEFT = 0;
constexpr int RIGHT = 1;
constexpr int UP = 2;
constexpr int DOWN = 3;

void ShipsGenerator::place_ships(std::vector<std::vector<std::shared_ptr<Tile>>>& field,
                                 std::vector<std::shared_ptr<Tile>>& ships, bool hide_ships) {
    std::mt19937 random;
    auto now = std::chrono::high_resolution_clock::now();
    random.seed(static_cast<unsigned>(now.time_since_epoch().count()));

    for (int ships_count = 1, decks = 4; ships_count <= 4; ships_count++, decks--) {
        for (int ship = 0; ship < ships_count; ship++) {
            bool success = false;
            while (!success) {
                success = true;
                int line = random() % field.size();
                int column = random() % field[line].size();
                int dir = random() % 4;

                if ((dir == LEFT && column < decks - 1) ||
                    (dir == RIGHT && column > field[line].size() - decks) ||
                    (dir == UP && line < decks - 1) ||
                    (dir == DOWN && line > field.size() - decks)) {
                    success = false;
                    continue;
                }

                int x_left = 0;
                int x_right = 0;
                int y_top = 0;
                int y_bottom = 0;

                switch (dir) {
                case LEFT:
                    x_left = std::max(0, column - decks);
                    x_right = std::min(static_cast<int>(field[line].size()) - 1, column + 1);
                    y_top = std::max(0, line - 1);
                    y_bottom = std::min(static_cast<int>(field.size()) - 1, line + 1);
                    break;
                case RIGHT:
                    x_left = std::max(0, column - 1);
                    x_right = std::min(static_cast<int>(field[line].size()) - 1, column + decks);
                    y_top = std::max(0, line - 1);
                    y_bottom = std::min(static_cast<int>(field.size()) - 1, line + 1);
                    break;
                case UP:
                    y_top = std::max(0, line - decks);
                    y_bottom = std::min(static_cast<int>(field.size()) - 1, line + 1);
                    x_left = std::max(0, column - 1);
                    x_right = std::min(static_cast<int>(field[line].size()) - 1, column + 1);
                    break;
                case DOWN:
                    y_top = std::max(0, line - 1);
                    y_bottom = std::min(static_cast<int>(field.size()) - 1, line + decks);
                    x_left = std::max(0, column - 1);
                    x_right = std::min(static_cast<int>(field[line].size()) - 1, column + 1);
                }


                for (int i = y_top; i <= y_bottom; i++) {
                    bool _break = false;
                    for (int j = x_left; j <= x_right; j++) {
                        if (field[i][j]->get_status() == Status::SHIP) {
                            _break = true;
                            success = false;
                            break;
                        }
                    }
                    if (_break)
                        break;
                }

                if (success) {
                    field[line][column]->set_dir(dir);
                    field[line][column]->set_len(decks);
                    ships.push_back(field[line][column]);
                    for (int i = 0; i < decks; i++) {
                        field[line][column]->change_status(Status::SHIP, hide_ships);
                        switch (dir) {
                        case LEFT: column--; break;
                        case RIGHT: column++; break;
                        case UP: line--; break;
                        case DOWN: line++; break;
                        default: break;
                        }
                    }
                }
            }
        }
    }
}
