#pragma once
#include "Field.h"
#include "Utils.h"
#include "ShipsGenerator.h"
#include "AI.h"

enum class GameState {
    SHIPREDACTOR,
    MAINLOOP,
    GAMEOVER
};

class PauseTable {
private:
    std::shared_ptr<Button> to_game;
    std::shared_ptr<Button> exit;
    sf::RectangleShape bg;
    sf::Text pause_text;
    std::vector<std::shared_ptr<Button>> buttons;

public:
    PauseTable();
    void show(sf::RenderWindow& window);
};

class Game {
private:
    Field first_field;
    Field second_field;
    ViewMode ShipRedactorLoop(sf::RenderWindow& window);
    ViewMode MainLoop(sf::RenderWindow& window);
    ViewMode GameOverLoop(sf::RenderWindow& window);

    void render_game(sf::RenderWindow& window, GameState state);

    bool pause;
    PauseTable pause_table;

    std::shared_ptr<Button> random_placement;
    std::shared_ptr<Button> start;

    sf::Text _4_deck_text;
    sf::Text _3_deck_text;
    sf::Text _2_deck_text;
    sf::Text _1_deck_text;

    bool ship_are_placed;
    bool click;

public:
    Game(size_t field_size);

    std::vector<std::shared_ptr<Button>> buttons;

    ViewMode Run(sf::RenderWindow& window);

    bool is_paused() {
        return pause;
    }

    void check_event(sf::Event& event);
    void check_click(sf::Vector2i mouse_pos);
    void check_dead(std::vector<std::vector<std::shared_ptr<Tile>>>& field, 
                    std::vector<std::shared_ptr<Tile>>& ships, int number);
};
