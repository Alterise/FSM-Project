#pragma once

#include <tinyfsm.hpp>
#include <SFML/Graphics.hpp>
#include <Character.h>

struct EyeContact       : tinyfsm::Event {
    std::shared_ptr<Character> guard;
    std::shared_ptr<Character> intruder;
};

struct Noise            : tinyfsm::Event {
    std::shared_ptr<Character> guard;
    sf::Vector2f location;
};

struct NextPatrolSide   : tinyfsm::Event {
    std::shared_ptr<Character> guard;
};

struct CamerasChecked   : tinyfsm::Event {
    std::shared_ptr<Character> guard;
    std::shared_ptr<Character> intruder;
};

struct NoiseChecked   : tinyfsm::Event {
    std::shared_ptr<Character> guard;
    std::shared_ptr<sf::CircleShape> noise;
};

struct IntruderArrested : tinyfsm::Event {
    std::shared_ptr<Character> guard;
};

struct IntruderLost : tinyfsm::Event {
    std::shared_ptr<Character> guard;
};

struct NewTick          : tinyfsm::Event {
    std::shared_ptr<Character> guard;
    std::shared_ptr<Character> intruder;
};