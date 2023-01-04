#pragma once

#include <tinyfsm.hpp>
#include <SFML/Graphics.hpp>
#include <Character.h>

struct EyeContact   : tinyfsm::Event {
    Character& character;
};

struct Noise        : tinyfsm::Event {
    sf::Vector2f location;
};
