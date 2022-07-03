#pragma once
#include <string>
#include "SFML/Graphics.hpp"
#include <iostream>

constexpr size_t WINDOW_WIDTH = 1600;
constexpr size_t WINDOW_HEIGHT = 720;

enum class ViewMode {
    NONE = -1,
    EXIT = 0,
    PLAY = 1,
    GAMEOVER = 2
};

class Utils {
public:
    static inline sf::Font font;
    static void init();
};

class Button {
private:
    sf::RectangleShape button;
    sf::Text text;
    ViewMode mode;
    bool function;

public:
    Button(const std::string& str, const sf::Vector2f& pos, ViewMode mode);
    sf::RectangleShape& get_rect() {
        return button;
    }
    sf::Text& get_text() {
        return text;
    }
    static ViewMode check_buttons(sf::Vector2i mouse_pos, std::vector<std::shared_ptr<Button>>& buttons);
    ViewMode get_mode();
    void set_size(sf::Vector2f size);
    void turn_on();
    void turn_off();
    bool is_pressed();
    void show(sf::RenderWindow& window);
};
