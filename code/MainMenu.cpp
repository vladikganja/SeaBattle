#include "MainMenu.h"

MainMenu::MainMenu() :  play(new Button("PLAY", { 800.f, 100.f }, ViewMode::PLAY)),
                        exit(new Button("EXIT", { 800.f, 340.f }, ViewMode::EXIT)) {
    bg_color = sf::Color(0, 122, 204);

    play->get_text().setCharacterSize(40);
    play->set_size({ 160.f, 60.f });
    play->get_rect().setPosition({ 720.f, 270.f });
    play->get_text().setPosition({ 748.f, 272.f });

    exit->get_text().setCharacterSize(40);
    exit->set_size({ 160.f, 60.f });
    exit->get_rect().setPosition({ 720.f, 390.f });
    exit->get_text().setPosition({ 748.f, 394.f });

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
        if (!Utils::block)
            to_return = Button::check_buttons(sf::Mouse::getPosition(window), buttons);
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
            Utils::block = false;
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
