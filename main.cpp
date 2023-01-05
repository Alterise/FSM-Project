#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <memory>
#include <unordered_set>

#include <Character.h>
#include <fsmlist.hpp>

sf::Color backgroundColor = sf::Color(104,202,0);
std::chrono::milliseconds dt = std::chrono::milliseconds(10);
std::chrono::time_point<std::chrono::steady_clock> timePoint;

sf::Vector2f guardRoomPosition = sf::Vector2f(50, 650);
sf::Vector2f arrestAreaPosition = sf::Vector2f(750, 650);
std::vector<sf::Vector2f> territoryCorners = {
        sf::Vector2f(150, 650),
        sf::Vector2f(150, 150),
        sf::Vector2f(650, 150),
        sf::Vector2f(650, 650)
};

int main() {
    std::shared_ptr<sf::RenderWindow> window(new sf::RenderWindow(sf::VideoMode(800, 800), "FSM Coursework"));
    float areaPadding = 100;
    auto area = sf::RectangleShape(sf::Vector2f(600, 600));
    area.setPosition(sf::Vector2f(areaPadding, areaPadding));
    area.setFillColor(sf::Color(194, 178, 128));

    auto arrestArea = sf::RectangleShape(sf::Vector2f(100, 100));
    arrestArea.setPosition(sf::Vector2f(700, 600));
    arrestArea.setFillColor(sf::Color(139, 69, 19));

    auto guardRoom = sf::RectangleShape(sf::Vector2f(100, 100));
    guardRoom.setPosition(sf::Vector2f(0, 600));
    guardRoom.setFillColor(sf::Color(100, 100, 100));

    auto guard = std::make_shared<Character>();
    guard->setPosition(sf::Vector2f(400, 400));
    guard->setDestination(sf::Vector2f(150, 650));
    guard->setColor(sf::Color(200, 0, 0));
    guard->setSpeed(100);

    std::shared_ptr<Character> intruder = nullptr;

    NewTick newTick;
    newTick.guard = guard;

    Noise noise;
    noise.guard = guard;

    EyeContact eyeContact;
    eyeContact.guard = guard;
    eyeContact.intruder = nullptr;

    NextPatrolSide nextPatrolSide;
    nextPatrolSide.guard = guard;

    CamerasChecked camerasChecked;
    camerasChecked.guard = guard;

    FsmList::start();

    while(window->isOpen()) {
        auto startTime = std::chrono::steady_clock::now();
        sendEvent(newTick);
        sf::Event event{};
        while(window->pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window->close();
            }

//            if(event.type == sf::Event::MouseButtonPressed) {
//                auto guardSize = guard->getSize();
//                guard->setDestination(sf::Vector2f(event.mouseButton.x - guardSize / 2, event.mouseButton.y - guardSize / 2));
//            }
        }

        if (isNear(*guard, territoryCorners[guard->getPatrolSide()])) {
            sendEvent(nextPatrolSide);
        }

        if (isNear(*guard, guardRoomPosition)) {
            if (guard->getState() == State::DONE) {
                guard->setState(State::IN_PROGRESS);
                timePoint = std::chrono::steady_clock::now();
            } else if (startTime - timePoint >= std::chrono::seconds(5)) {
                if (intruder != nullptr && onTerritory(*intruder)) {
                    camerasChecked.intruder = intruder;
                }
                sendEvent(camerasChecked);
            }
        }

        window->clear(backgroundColor);

        window->draw(area);
        window->draw(arrestArea);
        window->draw(guardRoom);
        guard->move();
        guard->drawSelf(window);

        window->display();
        std::this_thread::sleep_until(startTime + dt);
    }

    return 0;
}