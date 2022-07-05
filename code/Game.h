#pragma once
#include "Field.h"
#include "Utils.h"
#include "ShipsGenerator.h"
#include "AI.h"

enum class GameState {
    SHIPREDACTOR,
    MAINLOOP
};

class PauseTable {
private:
    std::shared_ptr<Button> to_game;
    std::shared_ptr<Button> exit;
    sf::Text pause_text;
    sf::Text win_text;
    std::vector<std::shared_ptr<Button>> buttons;

public:
    PauseTable();
    void set_winner(const std::string& winner);
    void show(sf::RenderWindow& window);
    std::vector<std::shared_ptr<Button>> get_buttons() {
        return buttons;
    }
};

class Game {
private:
    Field first_field;
    Field second_field;
    ViewMode ShipRedactorLoop(sf::RenderWindow& window);
    ViewMode MainLoop(sf::RenderWindow& window);

    void render_game(sf::RenderWindow& window, GameState state);

    static bool pause;
    static char winner;
    PauseTable pause_table;

    std::shared_ptr<Button> random_placement;
    std::shared_ptr<Button> start;

    bool ship_are_placed;
    bool click;

    sf::Text score_first_player;
    sf::Text score_second_player;
    sf::Text timer;
    sf::Text whose_turn;
    sf::Text total_score;

public:
    Game(size_t field_size);

    std::vector<std::shared_ptr<Button>> buttons;

    ViewMode Run(sf::RenderWindow& window);

    static bool is_paused() {
        return pause;
    }

    static char get_winner() {
        return winner;
    }
    void check_event(sf::Event& event);
    void check_click(sf::Vector2i mouse_pos);
    void check_dead(std::vector<std::vector<std::shared_ptr<Tile>>>& field, 
                    std::vector<std::shared_ptr<Tile>>& ships, int number);
    bool check_win(std::vector<std::shared_ptr<Tile>>& ships);

};
