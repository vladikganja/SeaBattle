#pragma once
#include "SFML/Graphics.hpp"
#include "Utils.h"

class MainMenu {
public:

    MainMenu();
    std::shared_ptr<Button> play;
    std::shared_ptr<Button> exit;

    ViewMode Run(sf::RenderWindow& window);
    void render_menu(sf::RenderWindow& window);

private:
    std::vector<std::shared_ptr<Button>> buttons;
    sf::Color bg_color;
};

