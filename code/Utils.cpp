#include "Utils.h"

bool Utils::block = false;

Button::Button(const std::string& str, const sf::Vector2f& pos, ViewMode _mode) {
    function = false;
    mode = _mode;
    button.setPosition(pos);
    button.setSize({ 80.f, 40.f });
    button.setFillColor(sf::Color(168, 114, 166));
    text.setFont(Utils::font);
    text.setString(str);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color(46, 36, 57));
    text.setOutlineThickness(1.f);
    text.setPosition({ pos.x + 20.f, pos.y + 10.f });
}

ViewMode Button::check_buttons(sf::Vector2i _mouse_pos, std::vector<std::shared_ptr<Button>>& buttons) {
    ViewMode to_return = ViewMode::NONE;
    sf::Vector2f mouse_pos = { static_cast<float>(_mouse_pos.x), static_cast<float>(_mouse_pos.y) };

    for (auto& button : buttons) {
        if (button->get_rect().getGlobalBounds().contains(mouse_pos)) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                to_return = button->get_mode();
                button->turn_on();
                break;
            }
        }
    }

    return to_return;
}

ViewMode Button::get_mode() {
    return mode;
}

void Button::set_size(sf::Vector2f size) {
    button.setSize(size);
}

void Button::turn_on() {
    function = true;
}

void Button::turn_off() {
    function = false;
}

bool Button::is_pressed() {
    return function;
}

void Button::show(sf::RenderWindow& window) {
    window.draw(button);
    window.draw(text);
}

void Utils::init() {
    font.loadFromFile("../../multimedia/CyrilicOld.TTF");
}
