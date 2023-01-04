#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <memory>

#include <Character.h>

sf::Color backgroundColor = sf::Color(104,202,0);
std::chrono::milliseconds dt = std::chrono::milliseconds(10);

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