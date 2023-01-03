#include <SFML/Graphics.hpp>
#include <tinyfsm.hpp>
#include <chrono>
#include <thread>
#include <memory>
#include <cmath>

sf::Color backgroundColor = sf::Color(104,202,0);
std::chrono::milliseconds dt = std::chrono::milliseconds(10);

//void move(const std::shared_ptr<sf::RenderWindow>& window, sf::Shape& object, const sf::Vector2<float>& shift,
//          const std::chrono::milliseconds& duration = std::chrono::milliseconds(500)) {
//    auto startTime = std::chrono::steady_clock::now();
//    auto framesCount = duration / std::chrono::milliseconds(1000 / 60);
//    auto frameDuration = duration / framesCount;
//    auto frameShift = shift / static_cast<float>(framesCount);
//    for (int i = 0; i < framesCount; ++i) {
//        object.setPosition(object.getPosition() + frameShift);
//        window->clear(backgroundColor);
//        window->draw(object);
//        window->display();
//        std::this_thread::sleep_for(frameDuration);
//    }
//}

class Character{
private:
    sf::CircleShape m_body;
    float m_size;
    sf::Vector2f m_destination;
    sf::Vector2f m_position;
    float m_speed;
    float m_tickSpeed;
public:
    Character() {
        m_size = 20.f;
        m_body = sf::CircleShape(m_size / 2);
        m_position = sf::Vector2f(0, 0);
        m_body.setPosition(m_position);
        m_body.setFillColor(sf::Color(0, 0, 0));
        m_speed = 100;
        m_tickSpeed = m_speed / (std::chrono::seconds(1) / std::chrono::milliseconds(10));
        m_destination = m_position;
    }

    void setSpeed(float speed) {
        m_speed = speed;
        m_tickSpeed = m_speed / (std::chrono::seconds(1) / std::chrono::milliseconds(10));
    }

    void setSize(float size) {
        m_size = size;
        m_body.setRadius(m_size / 2);
    }

    void setPosition(const sf::Vector2f& position) {
        m_position = position;
        m_body.setPosition(m_position);
        m_destination = m_position;
    }

    void setDestination(const sf::Vector2f& destination) {
        m_destination = destination;
    }

    void setColor(const sf::Color& color) {
        m_body.setFillColor(color);
    }

    float getSize() const {
        return m_size;
    }

    void move() {
        auto distance = m_destination - m_position;
        auto absDistance = std::sqrt(distance.x * distance.x + distance.y * distance.y);
        if (absDistance <= 1) {
            return;
        }
        m_position += distance * (m_tickSpeed / absDistance);
        m_body.setPosition(m_position);
    }

    const sf::CircleShape& getBody() {
        return m_body;
    }
private:

};

int main()
{
    std::shared_ptr<sf::RenderWindow> window(new sf::RenderWindow(sf::VideoMode(800, 800), "FSM Coursework"));
    float areaPadding = 100;
    auto area = sf::RectangleShape(static_cast<const sf::Vector2f>(
            static_cast<const sf::Vector2f>(window->getSize()) - sf::Vector2f(areaPadding * 2, areaPadding * 2)));
    area.setPosition(sf::Vector2f(areaPadding, areaPadding));
    area.setFillColor(sf::Color(194, 178, 128));
    Character guard;
    guard.setPosition(sf::Vector2f(120, 120));
    guard.setColor(sf::Color(200, 0, 0));
    guard.setSpeed(200);
    while(window->isOpen())
    {
        auto startTime = std::chrono::steady_clock::now();
        sf::Event event{};
        while(window->pollEvent(event))
        {
            if(event.type == sf::Event::Closed) {
                window->close();
            }

            if(event.type == sf::Event::MouseButtonPressed) {
                auto guardSize = guard.getSize();
                guard.setDestination(sf::Vector2f(event.mouseButton.x - guardSize / 2, event.mouseButton.y - guardSize / 2));
            }
        }

        window->clear(backgroundColor);

        window->draw(area);
        guard.move();
        window->draw(guard.getBody());

        window->display();
        std::this_thread::sleep_until(startTime + dt);
    }

    return 0;
}