#pragma once

#include "SFML/Graphics.hpp"
#include <chrono>
#include <cmath>
#include <optional>
#include <memory>
#include <states.hpp>

class Character{
private:
    sf::CircleShape m_body;
    float m_size;
    sf::Vector2f m_destination;
    sf::Vector2f m_position;
    float m_speed;
    float m_tickSpeed;
    std::shared_ptr<sf::CircleShape> m_target;
    int m_patrolCount;
    State m_state;

public:
    Character();

    void setSpeed(float speed);
    void setSize(float size);
    void setPosition(const sf::Vector2f& position);
    void setDestination(const sf::Vector2f& destination);
    void setDestinationArrest();
    void setTarget(const sf::CircleShape &target);
    void setState(State state);
    void resetTarget();
    void setColor(const sf::Color& color);

    float getSize() const;
    std::shared_ptr<sf::CircleShape> getTarget() const;
    State getState() const;
    int getPatrolCount() const;
    void incrementPatrolCount();
    const sf::CircleShape& getBody();

    void move();
private:

};