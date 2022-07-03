#include "MainMenu.h"

MainMenu::MainMenu() :  play(new Button("PLAY", { 600.f, 300.f }, ViewMode::PLAY)), 
                        exit(new Button("EXIT", {600.f, 350.f}, ViewMode::EXIT)) {
    bg_color = sf::Color(0, 122, 204);
    buttons.push_back(play);
    buttons.push_back(exit);
}

ViewMode MainMenu::Run(sf::RenderWindow& window) {
    ViewMode to_return = ViewMode::NONE;
    sf::Event event;
    window.pollEvent(event);
    event.type = sf::Event::GainedFocus;
    while (to_return == ViewMode::NONE) {
        window.pollEvent(event);
        to_return = Button::check_buttons(sf::Mouse::getPosition(window), buttons);
        if (event.type == sf::Event::Closed)
            to_return = ViewMode::EXIT;
        render_menu(window);
    }
    return to_return;
}

void MainMenu::render_menu(sf::RenderWindow& window) {
    window.clear(bg_color);
    for (auto& button : buttons) {
        button->show(window);
    }
    window.display();
}
