#include "Character.h"

Character::Character() {
    m_size = 20.f;
    m_position = sf::Vector2f(0, 0);
    m_body = sf::CircleShape(m_size / 2);
    m_body.setPosition(m_position);
    m_body.setFillColor(sf::Color(0, 0, 0));

    m_eyeSight = sf::CircleShape(100.f);
    m_eyeSight.setPosition(m_position);
    m_eyeSight.setFillColor(sf::Color(0, 0, 0, 50));
    m_speed = 100;
    m_tickSpeed = m_speed / (std::chrono::seconds(1) / std::chrono::milliseconds(10));
    m_destination = m_position;
    m_target = nullptr;
    m_state = State::DONE;
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
    m_destination = destination - sf::Vector2f(m_size / 2, m_size / 2);
}

void Character::setTarget(const sf::CircleShape& target) {
    m_target = std::make_shared<sf::CircleShape>(target);
}

void Character::resetTarget() {
    m_target = nullptr;
}

void Character::setColor(const sf::Color& color) {
    m_body.setFillColor(color);
    m_eyeSight.setFillColor(sf::Color(color.r, color.g, color.b, 50));
}

float Character::getSize() const {
    return m_size;
}

void Character::move() {
    m_eyeSight.setPosition(m_body.getPosition() - sf::Vector2f(m_eyeSight.getRadius() - m_size / 2, m_eyeSight.getRadius() - m_size / 2));
    if (m_target != nullptr) {
        m_destination = m_target->getPosition();
    }
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

//int Character::getPatrolCount() const {
//    return m_patrolCount;
//}

State Character::getState() const {
    return m_state;
}

//void Character::incrementPatrolCount() {
//    m_patrolCount++;
//}

void Character::setState(const State state) {
    m_state = state;
}

std::shared_ptr<sf::CircleShape> Character::getTarget() const {
    return m_target;
}

void Character::setDestinationArrest() {
    setDestination(sf::Vector2f(750, 650));
}

void Character::drawSelf(const std::shared_ptr<sf::RenderWindow>& window) {
    window->draw(m_body);
    window->draw(m_eyeSight);
}