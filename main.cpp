#include <SFML/Graphics.hpp>
#include <tinyfsm.hpp>
#include <chrono>
#include <thread>

using namespace sf;

void move(RenderWindow& window, Shape& object, const sf::Vector2<float>& shift,
          const std::chrono::milliseconds& duration = std::chrono::milliseconds(500)) {
    auto startTime = std::chrono::steady_clock::now();
    auto framesCount = duration / std::chrono::milliseconds(1000 / 60);
    auto frameDuration = duration / framesCount;
    auto frameShift = shift / static_cast<float>(framesCount);
    for (int i = 0; i < framesCount; ++i) {
        object.setPosition(object.getPosition() + frameShift);
        window.clear(Color(120, 200, 30, 0));
        window.draw(object);
        window.display();
        std::this_thread::sleep_for(frameDuration);
    }
}

enum class Object {
    NOTHING,
    CONTAINER,
    GUARD,
    OFFENDER
};

class Playground {
public:
    int m_Padding;
    int m_Height;
    int m_Width;
private:

public:
    Playground() {
        m_Padding = 50;
        m_Height = 10;
        m_Width = 10;
        m_Data = std::vector<std::vector<Object>>(m_Height, std::vector<Object>(m_Width, Object::NOTHING));
    }

    std::vector<std::vector<Object>> m_Data;
};

int main()
{
    RenderWindow window(VideoMode(800, 800), "FSM Coursework");
    CircleShape circle(50.f);

    while (window.isOpen())
    {
        auto startTime = std::chrono::steady_clock::now();
        Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        window.clear(Color(120, 200, 30, 0));

//        if (event.type == Event::TextEntered) {
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
//                move(window, circle, Vector2<float>{0, -50});
//            }
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
//                move(window, circle, Vector2<float>{-50, 0});
//            }
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
//                move(window, circle, Vector2<float>{0, 50});
//            }
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
//                move(window, circle, Vector2<float>{50, 0});
//            }
//        }

        circle.setFillColor(Color(200, 0, 0));

        window.draw(circle);

        window.display();
    }

    return 0;
}