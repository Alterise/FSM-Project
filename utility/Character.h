#pragma once

#include "SFML/Graphics.hpp"
#include <chrono>
#include <cmath>

class Character{
private:
    sf::CircleShape m_body;
    float m_size;
    sf::Vector2f m_destination;
    sf::Vector2f m_position;
    float m_speed;
    float m_tickSpeed;
public:
    Character();

    void setSpeed(float speed);
    void setSize(float size);
    void setPosition(const sf::Vector2f& position);
    void setDestination(const sf::Vector2f& destination);
    void setColor(const sf::Color& color);

    float getSize() const;
    const sf::CircleShape& getBody();

    void move();
private:

};