#pragma once

#include <tinyfsm.hpp>
#include <SFML/Graphics.hpp>
#include <Character.h>

struct EyeContact   : tinyfsm::Event {
    std::shared_ptr<Character> guard;
    std::shared_ptr<Character> intruder;
};

struct Noise        : tinyfsm::Event {
    std::shared_ptr<Character> guard;
    sf::Vector2f location;
};

struct NewTick      : tinyfsm::Event {
    std::shared_ptr<Character> guard;
};