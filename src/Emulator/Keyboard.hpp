#pragma once

#include <SFML/Window/Keyboard.hpp>

namespace c8emu {

enum Chip8Key : unsigned char
{
    C8_KEY_1, C8_KEY_2, C8_KEY_3, C8_KEY_C,
    C8_KEY_4, C8_KEY_5, C8_KEY_6, C8_KEY_D,
    C8_KEY_7, C8_KEY_8, C8_KEY_9, C8_KEY_E,
    C8_KEY_A, C8_KEY_0, C8_KEY_B, C8_KEY_F
};

constexpr sf::Keyboard::Key C8_KEYS[] =
{
    sf::Keyboard::Key::Num1, sf::Keyboard::Key::Num2, sf::Keyboard::Key::Num3, sf::Keyboard::Key::Num4,
    sf::Keyboard::Key::Q,    sf::Keyboard::Key::W,    sf::Keyboard::Key::E,    sf::Keyboard::Key::R,
    sf::Keyboard::Key::A,    sf::Keyboard::Key::S,    sf::Keyboard::Key::D,    sf::Keyboard::Key::F,
    sf::Keyboard::Key::Z,    sf::Keyboard::Key::X,    sf::Keyboard::Key::C,    sf::Keyboard::Key::V,
};

}
