#pragma once

#include "SFML/Graphics.hpp"
#include <chrono>
#include <cmath>
#include <optional>
#include <memory>
#include <states.hpp>

class Character;

bool isNear(const Character& character, sf::Vector2f location);
bool isNearNoise(const Character& character, sf::Vector2f location);
bool isInEyeSight(const Character& character1, const Character& character2);
bool onTerritory(const Character& character);

class Character{
private:
    sf::CircleShape m_body;
    sf::CircleShape m_eyeSight;
    float m_size;
    sf::Vector2f m_destination;
    sf::Vector2f m_position;
    float m_speed;
    float m_tickSpeed;
    std::shared_ptr<Character> m_target;
    std::shared_ptr<Character> m_binded;
    int m_patrolSide;
    int m_patrolCount;
    State m_state;

public:
    Character();

    void setSpeed(float speed);
    void setSize(float size);
    void setPosition(const sf::Vector2f& position);
    void setDestination(const sf::Vector2f& destination);
    void setTarget(const std::shared_ptr<Character> &target);
    void setBinded(const std::shared_ptr<Character> &binded);
    void setState(State state);
    void resetTarget();
    void resetBinded();
    void resetPatrol();
    void setColor(const sf::Color& color);

    float getSize() const;
    sf::Vector2f getPosition() const;
    std::shared_ptr<Character> getTarget() const;
    State getState() const;
    int getPatrolCount() const;
    void incrementPatrolCount();
    int getPatrolSide() const;
    void incrementPatrolSide();
    const sf::CircleShape& getBody();

    void move();
    void drawSelf(const std::shared_ptr<sf::RenderWindow>& window);
};