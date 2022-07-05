#include "Game.h"

bool Game::pause = false;
char Game::winner = 0;

Game::Game(size_t field_size) : random_placement(new Button("Random placement", { 900.f, 50.f }, ViewMode::NONE)),
start(new Button("Drown them all!!!", { 900.f, 570.f }, ViewMode::PLAY)) {
    if (field_size < 10) {
        field_size = 10;
    }
    if (field_size > 20) {
        field_size = 20;
    }

    buttons.push_back(random_placement);
    buttons.push_back(start);

    for (auto& button : buttons) {
        button->set_size({ 500.f, 100.f });
        button->get_rect().setFillColor(sf::Color(49, 140, 231));
        button->get_rect().setOutlineThickness(5.f);
        button->get_rect().setOutlineColor(sf::Color(89, 236, 255));
        button->get_text().setCharacterSize(60);
    }

    ship_are_placed = false;
    click = false;
    first_field.generate(field_size, false);
    second_field.generate(field_size, true);
}

// SHIP REDACTOR //////////////////////////////////////////////////////////////////////////////////

ViewMode Game::ShipRedactorLoop(sf::RenderWindow& window) {
    ViewMode to_return = ViewMode::NONE;
    sf::Event event;
    window.pollEvent(event);
    event.type = sf::Event::GainedFocus;

    while (to_return == ViewMode::NONE) {
        window.pollEvent(event);

        if (!pause) {
            to_return = Button::check_buttons(sf::Mouse::getPosition(window), buttons);

            if (random_placement->is_pressed()) {
                ship_are_placed = true;
                first_field.reset();
                ShipsGenerator::place_ships(first_field.get_field(), first_field.get_ships(), false);
                random_placement->turn_off();
            }

            if (start->is_pressed()) {
                if (!ship_are_placed) {
                    to_return = ViewMode::NONE;
                }
                else {
                    second_field.reset();
                    ShipsGenerator::place_ships(second_field.get_field(), second_field.get_ships(), true);
                }
                start->turn_off();
            }
        }
        else {
            to_return = Button::check_buttons(sf::Mouse::getPosition(window), pause_table.get_buttons());
            if (pause_table.get_buttons()[0]->is_pressed()) {
                pause = false;
                click = true;
                pause_table.get_buttons()[0]->turn_off();
            }
            if (pause_table.get_buttons()[1]->is_pressed()) {
                Utils::block = true;
            }
        }

        if (event.type == sf::Event::Closed)
            to_return = ViewMode::EXIT;

        check_event(event);

        render_game(window, GameState::SHIPREDACTOR);
    }

    return to_return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ViewMode Game::MainLoop(sf::RenderWindow& window) {
    ViewMode to_return = ViewMode::NONE;
    sf::Event event;
    window.pollEvent(event);
    event.type = sf::Event::GainedFocus;
    while (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        continue;
    }

    while (to_return == ViewMode::NONE) {
        window.pollEvent(event);

        if (!pause) {
            bool old_click = click;
            check_click(sf::Mouse::getPosition(window));

            if (!old_click && click) {
                AI::make_turn(first_field.get_field());
            }

            check_dead(first_field.get_field(), first_field.get_ships(), 1);
            check_dead(second_field.get_field(), second_field.get_ships(), 2);

            if (check_win(first_field.get_ships())) {
                std::cout << "Player 2 wins!\n";
                pause = true;
                winner = 2;
                pause_table.set_winner("Player 2");
            }

            if (check_win(second_field.get_ships())) {
                std::cout << "Player 1 wins!\n";
                pause = true;
                winner = 1;
                pause_table.set_winner("Player 1");
            }
        }
        else {
            to_return = Button::check_buttons(sf::Mouse::getPosition(window), pause_table.get_buttons());
            if (pause_table.get_buttons()[0]->is_pressed()) {
                pause = false;
                click = true;
                pause_table.get_buttons()[0]->turn_off();
                if (winner != 0) {
                    winner = 0;
                    first_field.reset();
                    second_field.reset();
                    return ViewMode::NONE;
                }
            }
            if (pause_table.get_buttons()[1]->is_pressed()) {
                Utils::block = true;
            }
        }

        if (event.type == sf::Event::Closed)
            to_return = ViewMode::EXIT;

        if (winner == 0)
            check_event(event);

        render_game(window, GameState::MAINLOOP);
    }

    return to_return;
}

void Game::render_game(sf::RenderWindow& window, GameState state) {
    window.clear(sf::Color(110, 110, 110));

    switch (state) {
    case GameState::SHIPREDACTOR:
    {
        first_field.show(window);
        for (auto& button : buttons) {
            button->show(window);
        }
    }
    break;

    case GameState::MAINLOOP:
    {
        first_field.show(window);
        second_field.show(window);
    }
    break;

    default:
        break;
    }

    if (pause) {
        pause_table.show(window);
    }

    window.display();
}

ViewMode Game::Run(sf::RenderWindow& window) {
    ViewMode to_return = ViewMode::NONE;

    while (true) {
        AI::reset();
        pause = false;
        winner = 0;

        to_return = ShipRedactorLoop(window);

        if (to_return != ViewMode::NONE && to_return != ViewMode::PLAY)
            break;

        to_return = MainLoop(window);

        if (to_return != ViewMode::NONE)
            break;
    }
    
    return to_return;
}

void Game::check_event(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        case sf::Keyboard::Escape:
            pause = !pause;
            break;
        default:
            break;
        }
    }
}

void Game::check_click(sf::Vector2i _mouse_pos) {
    if (click && !sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        click = false;
        return;
    }

    sf::Vector2f mouse_pos = { static_cast<float>(_mouse_pos.x), static_cast<float>(_mouse_pos.y) };
    for (int i = 0; i < second_field.get_field().size(); i++) {
        for (int j = 0; j < second_field.get_field()[i].size(); j++) {
            auto coord = second_field.get_field()[i][j]->get_coord();
            if (coord.contains(mouse_pos)) {
                if (!click && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    click = true;
                    switch (second_field.get_field()[i][j]->get_status()) {
                    case Status::VOID:
                        second_field.get_field()[i][j]->change_status(Status::MISS);
                        break;
                    case Status::SHIP:
                        second_field.get_field()[i][j]->change_status(Status::HURT);
                        break;
                    default:
                        click = false;
                        break;
                    }
                }
            }
        }
    }
}

void Game::check_dead(std::vector<std::vector<std::shared_ptr<Tile>>>& field,
    std::vector<std::shared_ptr<Tile>>& ships, int number) {
    for (auto& tile : ships) {
        if (tile->get_status() == Status::DEAD) {
            continue;
        }

        auto field_size = second_field.get_field().size();
        bool dead = true;
        auto dir = tile->get_dir();
        auto len = tile->get_len();

        int line = 0, column = 0;
        if (number == 1) {
            line = static_cast<int>((tile->get_id()) / field_size);
            column = static_cast<int>((tile->get_id()) % field_size);
        }
        else {
            line = static_cast<int>((tile->get_id() - field_size * field_size) / field_size);
            column = static_cast<int>((tile->get_id() - field_size * field_size) % field_size);
        }

        int delta_x = 0, delta_y = 0;
        switch (dir) {
        case 0: delta_x = -1; delta_y = 0; break;
        case 1: delta_x = 1; delta_y = 0; break;
        case 2: delta_x = 0; delta_y = -1; break;
        case 3: delta_x = 0; delta_y = 1; break;
        default: break;
        }

        for (int i = 0; i < len; i++) {
            dead &= field[line][column]->get_status() == Status::HURT;
            line += delta_y;
            column += delta_x;
        }

        if (dead) {
            for (int i = 0; i < len; i++) {
                line -= delta_y;
                column -= delta_x;
                field[line][column]->change_status(Status::DEAD);
            }
        }
    }
}

bool Game::check_win(std::vector<std::shared_ptr<Tile>>& ships) {
    for (auto& el : ships) {
        if (el->get_status() != Status::DEAD) {
            return false;
        }
    }
    return true;
}

// PAUSE TABLE ////////////////////////////////////////////////////////////////////////////////////

PauseTable::PauseTable() : to_game(new Button("Back to game", { 800.f, 100.f }, ViewMode::NONE)),
exit(new Button("Quit", { 800.f, 340.f }, ViewMode::MENU)) {
    to_game->get_text().setCharacterSize(24);
    to_game->set_size({160.f, 60.f});
    to_game->get_rect().setPosition({720.f, 270.f});
    to_game->get_text().setPosition({735.f, 283.f});

    exit->get_text().setCharacterSize(24);
    exit->set_size({ 160.f, 60.f });
    exit->get_rect().setPosition({720.f, 390.f});
    exit->get_text().setPosition({780.f, 403.f});

    buttons.push_back(to_game);
    buttons.push_back(exit);

    win_text.setFont(Utils::font);
    win_text.setCharacterSize(24);
    win_text.setFillColor(sf::Color(255, 255, 255));
    win_text.setOutlineThickness(1.f);
    win_text.setPosition({730.f, 343.f});
}

void PauseTable::set_winner(const std::string& winner) {
    win_text.setString(winner + " wins!");
}

void PauseTable::show(sf::RenderWindow& window) {
    to_game->show(window);
    for (auto& button : buttons) {
        button->show(window);
    }
    if (Game::get_winner() != 0) {
        window.draw(win_text);
    }
}
