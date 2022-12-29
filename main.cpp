#include <SFML/Graphics.hpp>
#include <tinyfsm.hpp>
#include <chrono>
#include <thread>
#include <memory>

using namespace sf;

Color backgroundColor = Color(150, 250, 150);

// TODO: добавить отрисовку внешних фигур
void move(const std::shared_ptr<RenderWindow>& window, Shape& object, const sf::Vector2<float>& shift,
          const std::chrono::milliseconds& duration = std::chrono::milliseconds(500)) {
    auto startTime = std::chrono::steady_clock::now();
    auto framesCount = duration / std::chrono::milliseconds(1000 / 60);
    auto frameDuration = duration / framesCount;
    auto frameShift = shift / static_cast<float>(framesCount);
    for (int i = 0; i < framesCount; ++i) {
        object.setPosition(object.getPosition() + frameShift);
        window->clear(backgroundColor);
        window->draw(object);
        window->display();
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
private:
    int m_Padding;
    int m_Height;
    int m_Width;
    std::shared_ptr<RenderWindow> m_window;
    RectangleShape shape;

    std::vector<std::vector<Object>> m_Data;
public:
    explicit Playground(std::shared_ptr<RenderWindow>& window) {
        m_window = window;
        m_Padding = 50;
        m_Height = 10;
        m_Width = 10;
        m_Data = std::vector<std::vector<Object>>(m_Height, std::vector<Object>(m_Width, Object::NOTHING));
        Vector2f windowSize = static_cast<const Vector2f>(m_window->getSize());
        shape = RectangleShape(static_cast<const Vector2f>(windowSize - Vector2f(m_Padding * 2, m_Padding * 2)));
        shape.setFillColor(Color(240,230,140));
        shape.setPosition(Vector2f(m_Padding, m_Padding));
    }

    void draw() {
        m_window->draw(shape);
    }
};

int main()
{
    std::shared_ptr<RenderWindow> window(new RenderWindow(VideoMode(800, 800), "FSM Coursework"));
    CircleShape circle(50.f);
    auto pg = Playground(window);
    while (window->isOpen())
    {
        auto startTime = std::chrono::steady_clock::now();
        Event event{};
        while (window->pollEvent(event))
        {
            if (event.type == Event::Closed) {
                window->close();
            }
        }

        window->clear(backgroundColor);

        if (event.type == Event::TextEntered) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                move(window, circle, Vector2<float>{0, -50});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                move(window, circle, Vector2<float>{-50, 0});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                move(window, circle, Vector2<float>{0, 50});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                move(window, circle, Vector2<float>{50, 0});
            }
        }

        circle.setFillColor(Color(200, 0, 0));

        pg.draw();
        window->draw(circle);

        window->display();
    }

    return 0;
}