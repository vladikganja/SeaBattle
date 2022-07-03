#include "Game.h"
#include "MainMenu.h"

int main() {
    std::shared_ptr<sf::RenderWindow> window(new sf::RenderWindow{ 
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SeaBattle", sf::Style::Close });

    Utils::init();

    MainMenu menu;
    ViewMode mode = ViewMode::NONE;

    while (mode != ViewMode::EXIT) {
        mode = menu.Run(*window);
        size_t field_size = 10; // have to make slider
        if (mode == ViewMode::PLAY) {
            Game game(field_size);
            mode = game.Run(*window);
        }
    }

    return 0;
}
