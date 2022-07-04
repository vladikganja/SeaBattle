#include "AI.h"

int AI::last_line = -1;
int AI::last_column = -1;
bool AI::random = true;

void AI::make_turn(std::vector<std::vector<std::shared_ptr<Tile>>>& field) {
    std::mt19937 random;
    auto now = std::chrono::high_resolution_clock::now();
    random.seed(static_cast<unsigned>(now.time_since_epoch().count()));

    int line = 0, column = 0;
    int field_size = static_cast<int>(field.size());

    if (last_line != -1 && last_column != -1 && 
        field[last_line][last_column]->get_status() == Status::DEAD) {
        AI::random = true;
    }

    if (AI::random) {
        bool success = false;
        int efforts_count = 0;
        while (!success && efforts_count < field_size * field_size) {
            efforts_count++;
            success = true;
            line = random() % field_size;
            column = random() % field_size;

            if (field[line][column]->get_status() == Status::MISS || field[line][column]->get_status() == Status::DEAD) {
                success = false;
                continue;
            }

            else if (field[line][column]->get_status() == Status::VOID || field[line][column]->get_status() == Status::SHIP) {
                int x_left = std::max(0, column - 1);
                int x_right = std::min(field_size - 1, column + 1);
                int y_top = std::max(0, line - 1);
                int y_bottom = std::min(field_size - 1, line + 1);

                for (int i = y_top; i <= y_bottom; i++) {
                    bool _break = false;
                    for (int j = x_left; j <= x_right; j++) {
                        if (field[i][j]->get_status() == Status::DEAD) {
                            success = false;
                            _break = true;
                            break;
                        }
                    }
                    if (_break) {
                        break;
                    }
                }
            }            
        }

        if (efforts_count == field_size * field_size) {
            return;
        }

        switch (field[line][column]->get_status()) {
        case Status::VOID:
            field[line][column]->change_status(Status::MISS);
            break;
        case Status::SHIP:
            field[line][column]->change_status(Status::HURT);
            last_line = line;
            last_column = column;
            AI::random = false;
            break;
        default:
            break;
        }

        std::cout << line << ' ' << column << '\n';
    }
    else {

        bool left = true;
        bool right = true;
        bool top = true;
        bool bottom = true;

        // Check if there are dead ships in the 2nd radius

        int top_line = last_line - 2;
        int bottom_line = last_line + 2;
        for (int i = std::max(0, last_column - 1); i <= std::min(field_size - 1, last_column + 1); i++) {
            if (top_line > 1 && field[top_line][i]->get_status() == Status::DEAD) {
                top = false;
            }
            if (bottom_line < field_size - 2 && field[bottom_line][i]->get_status() == Status::DEAD) {
                bottom = false;
            }
        }

        int left_line = last_column - 2;
        int right_line = last_column + 2;
        for (int i = std::max(0, last_line - 1); i <= std::min(field_size - 1, last_line + 1); i++) {
            if (left_line > 1 && field[i][left_line]->get_status() == Status::DEAD) {
                left = false;
            }
            if (right_line < field_size - 2 && field[i][right_line]->get_status() == Status::DEAD) {
                right = false;
            }
        }

        // Check if there is an exact direction of a ship

        int left_tile = last_column - 1;
        int right_tile = last_column + 1;
        int top_tile = last_line - 1;
        int bottom_tile = last_line + 1;

        if ((left_tile >= 0 && field[last_line][left_tile]->get_status() == Status::HURT) ||
            (right_tile <= field_size - 1 && field[last_line][right_tile]->get_status() == Status::HURT)) {
            top = false;
            bottom = false;
        }

        if ((top_tile >= 0 && field[top_tile][last_column]->get_status() == Status::HURT) ||
            (bottom_tile <= field_size - 1 && field[bottom_tile][last_column]->get_status() == Status::HURT)) {
            left = false;
            right = false;
        }

        if ((left_tile >= 0 && field[last_line][left_tile]->get_status() == Status::MISS) || left_tile < 0) {
            left = false;
        }
        if ((right_tile <= field_size - 1 && field[last_line][right_tile]->get_status() == Status::MISS) || 
            right_tile > field_size - 1) {
            right = false;
        }
        if ((top_tile >= 0 && field[top_tile][last_column]->get_status() == Status::MISS) || top_tile < 0) {
            top = false;
        }
        if ((bottom_tile <= field_size - 1 && field[bottom_tile][last_column]->get_status() == Status::MISS) || 
            bottom_tile > field_size - 1) {
            bottom = false;
        }

        // Final check of chosen directions

        std::vector<sf::Vector2i> chosen_tiles;

        if (left) {
            int tmp_column = left_tile;
            while (field[last_line][tmp_column]->get_status() == Status::HURT) {
                tmp_column--;
                if (tmp_column < 0) {
                    left = false;
                    break;
                }
            }

            if (left) {
                if (field[last_line][tmp_column]->get_status() == Status::MISS) {
                    left = false;
                }
                else {
                    for (int i = std::max(0, last_line - 1); i <= std::min(field_size - 1, last_line + 1); i++) {
                        int tmp_left_line = tmp_column - 1;
                        if (tmp_left_line >= 0 && field[i][tmp_left_line]->get_status() == Status::DEAD) {
                            left = false;
                        }
                    }
                }
            }

            if (left) {
                chosen_tiles.push_back({last_line, tmp_column});
            }
        }

        if (right){
            int tmp_column = right_tile;
            while (field[last_line][tmp_column]->get_status() == Status::HURT) {
                tmp_column++;
                if (tmp_column > field_size - 1) {
                    right = false;
                    break;
                }
            }

            if (right) {
                if (field[last_line][tmp_column]->get_status() == Status::MISS) {
                    right = false;
                }
                else {
                    for (int i = std::max(0, last_line - 1); i <= std::min(field_size - 1, last_line + 1); i++) {
                        int tmp_right_line = tmp_column + 1;
                        if (tmp_right_line <= field_size - 1 && field[i][tmp_right_line]->get_status() == Status::DEAD) {
                            left = false;
                        }
                    }
                }
            }

            if (right) {
                chosen_tiles.push_back({ last_line, tmp_column });
            }
        }

        if (top) {
            int tmp_line = top_tile;
            while (field[tmp_line][last_column]->get_status() == Status::HURT) {
                tmp_line--;
                if (tmp_line < 0) {
                    top = false;
                    break;
                }
            }

            if (top) {
                if (field[tmp_line][last_column]->get_status() == Status::MISS) {
                    top = false;
                }
                else {
                    for (int i = std::max(0, last_column - 1); i <= std::min(field_size - 1, last_column + 1); i++) {
                        int tmp_top_line = tmp_line - 1;
                        if (tmp_top_line >= 0 && field[tmp_top_line][i]->get_status() == Status::DEAD) {
                            top = false;
                        }
                    }
                }
            }

            if (top) {
                chosen_tiles.push_back({ tmp_line, last_column });
            }
        }

        if (bottom) {
            int tmp_line = bottom_tile;
            while (field[tmp_line][last_column]->get_status() == Status::HURT) {
                tmp_line++;
                if (tmp_line > field_size - 1) {
                    bottom = false;
                    break;
                }
            }

            if (bottom) {
                if (field[tmp_line][last_column]->get_status() == Status::MISS) {
                    bottom = false;
                }
                else {
                    for (int i = std::max(0, last_column - 1); i <= std::min(field_size - 1, last_column + 1); i++) {
                        int tmp_bottom_line = tmp_line - 1;
                        if (tmp_bottom_line <= field_size - 1 && field[tmp_bottom_line][i]->get_status() == Status::DEAD) {
                            bottom = false;
                        }
                    }
                }
            }

            if (bottom) {
                chosen_tiles.push_back({ tmp_line, last_column });
            }
        }

        // Chosing of tile

        if (chosen_tiles.size() > 0) {
            int index = random() % chosen_tiles.size();

            int chosen_column = chosen_tiles[index].y; // y and x are inverted, that's fine
            int chosen_line = chosen_tiles[index].x;

            switch (field[chosen_line][chosen_column]->get_status()) {
            case Status::VOID:
                field[chosen_line][chosen_column]->change_status(Status::MISS);
                break;
            case Status::SHIP:
                field[chosen_line][chosen_column]->change_status(Status::HURT);
                last_column = chosen_column;
                last_line = chosen_line;
                break;
            default:
                break;
            }

            std::cout << chosen_line << ' ' << chosen_column << '\n';
        }
        else {
            throw std::invalid_argument("Impossible to make turn!");
        }
    }
}

void AI::reset() {
    random = true;
    last_column = -1;
    last_line = -1;
}
