#include "Character.h"

Character::Character() {
    m_size = 20.f;
    m_body = sf::CircleShape(m_size / 2);
    m_position = sf::Vector2f(0, 0);
    m_body.setPosition(m_position);
    m_body.setFillColor(sf::Color(0, 0, 0));
    m_speed = 100;
    m_tickSpeed = m_speed / (std::chrono::seconds(1) / std::chrono::milliseconds(10));
    m_destination = m_position;
}

void Character::setSpeed(float speed) {
    m_speed = speed;
    m_tickSpeed = m_speed / (std::chrono::seconds(1) / std::chrono::milliseconds(10));
}

void Character::setPosition(const sf::Vector2f& position) {
    m_position = position;
    m_body.setPosition(m_position);
    m_destination = m_position;
}

void Character::setDestination(const sf::Vector2f& destination) {
    m_destination = destination;
}

void Character::setColor(const sf::Color& color) {
    m_body.setFillColor(color);
}

float Character::getSize() const {
    return m_size;
}

void Character::move() {
    auto distance = m_destination - m_position;
    auto absDistance = std::sqrt(distance.x * distance.x + distance.y * distance.y);
    if (absDistance <= 1) {
        return;
    }
    m_position += distance * (m_tickSpeed / absDistance);
    m_body.setPosition(m_position);
}

const sf::CircleShape& Character::getBody() {
    return m_body;
}

void Character::setSize(float size) {
    m_size = size;
    m_body.setRadius(m_size / 2);
}
