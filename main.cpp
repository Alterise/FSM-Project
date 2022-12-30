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
    float m_Padding;
    int m_Height;
    int m_Width;
    std::shared_ptr<RenderWindow> m_window;
    RectangleShape m_shape;
    std::vector<RectangleShape> m_staticShapes;

    std::vector<std::vector<Object>> m_Data;
public:
    explicit Playground(std::shared_ptr<RenderWindow>& window) {
        m_window = window;
        m_Padding = 50;
        m_Height = 10;
        m_Width = 10;
        m_Data = std::vector<std::vector<Object>>(m_Height, std::vector<Object>(m_Width, Object::NOTHING));
        Vector2f windowSize = static_cast<const Vector2f>(m_window->getSize());
        m_shape = RectangleShape(static_cast<const Vector2f>(windowSize - Vector2f(m_Padding * 2, m_Padding * 2)));
        m_shape.setFillColor(Color(240, 230, 140));
        m_shape.setPosition(Vector2f(m_Padding, m_Padding));
        float lineWidth = 5;
        auto playgroundSize = m_shape.getSize();
        for (int i = 0; i <= m_Height; ++i) {
            auto line = RectangleShape(Vector2f(playgroundSize.x + lineWidth / 2, lineWidth));
            line.setPosition(m_Padding, m_Padding - lineWidth / 5 + i * (playgroundSize.y / m_Height));
            line.setFillColor(Color(0, 0, 0));
            m_staticShapes.push_back(line);
        }

        for (int i = 0; i <= m_Width; ++i) {
            auto line = RectangleShape(Vector2f(lineWidth, playgroundSize.y + lineWidth / 2));
            line.setPosition(m_Padding - lineWidth / 5 + i * (playgroundSize.x / m_Height), m_Padding);
            line.setFillColor(Color(0, 0, 0));
            m_staticShapes.push_back(line);
        }
    }

    void draw() {
        m_window->draw(m_shape);
        for (const auto &shape: m_staticShapes) {
            m_window->draw(shape);
        }
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