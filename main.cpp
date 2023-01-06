#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <memory>
#include <unordered_set>
#include <iostream>

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

    auto noiseEffect = std::make_shared<sf::CircleShape>(30.f);
    noiseEffect->setFillColor(sf::Color(200, 0, 0, 150));
    noiseEffect->setPosition(sf::Vector2f(-100, -100));
    int noiseState = 0;

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

    NoiseChecked noiseChecked;
    noiseChecked.guard = guard;
    noiseChecked.noise = noiseEffect;

    IntruderArrested intruderArrested;
    intruderArrested.guard = guard;

    IntruderLost intruderLost;
    intruderLost.guard = guard;

    FsmList::start();

    while(window->isOpen()) {
        auto startTime = std::chrono::steady_clock::now();
        sendEvent(newTick);
        sf::Event event{};
        while(window->pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window->close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    if (intruder != nullptr && onTerritory(*intruder)) {
                        auto intruderSize = intruder->getSize();
                        auto location = intruder->getPosition() + sf::Vector2f(intruderSize / 2, intruderSize / 2);
                        noise.location = location;
                        sendEvent(noise);
                        noiseState++;
                        auto noiseSize = noiseEffect->getRadius();
                        noiseEffect->setPosition(location - sf::Vector2f(noiseSize, noiseSize));
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (intruder != nullptr) {
                    if (intruder->getState() != State::INTRUDER_ARRESTING) {
                        auto intruderSize = intruder->getSize();
                        intruder->setDestination(
                                sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                    }
                } else {
                    intruder = std::make_shared<Character>();
                    auto intruderSize = intruder->getSize();
                    intruder->setPosition(
                            sf::Vector2f(event.mouseButton.x - intruderSize / 2,
                                         event.mouseButton.y - intruderSize / 2));
                    intruder->setColor(sf::Color(0, 0, 200));
                    intruder->setSpeed(250);
                    intruder->setState(State::INSTRUDER_FREE);
                }
            }
        }

        if (isNear(*guard, territoryCorners[guard->getPatrolSide()])) {
            sendEvent(nextPatrolSide);
        }

        if (intruder != nullptr) {
            if (intruder->getState() == State::INSTRUDER_FREE && isInEyeSight(*guard, *intruder) && onTerritory(*intruder)) {
                eyeContact.intruder = intruder;
                newTick.intruder = intruder;
                intruder->setState(State::INTRUDER_CHASING);
                sendEvent(eyeContact);
            }

            if (intruder->getState() == State::INTRUDER_CHASING && !onTerritory(*intruder)) {
                intruder->setState(State::INSTRUDER_FREE);
                sendEvent(intruderLost);
            }
        }

        if (isNear(*guard, guardRoomPosition)) {
            if (guard->getState() == State::GUARD_DONE) {
                guard->setState(State::GUARD_IN_PROGRESS);
                timePoint = std::chrono::steady_clock::now();
            } else if (startTime - timePoint >= std::chrono::seconds(5)) {
                if (intruder != nullptr && onTerritory(*intruder)) {
                    camerasChecked.intruder = intruder;
                }
                sendEvent(camerasChecked);
            }
        }

        if (isNearNoise(*guard, noiseEffect->getPosition())) {
            sendEvent(noiseChecked);
        }

        if (isNear(*guard, arrestAreaPosition)) {
            intruder = nullptr;
            camerasChecked.intruder = nullptr;
            newTick.intruder = nullptr;
            eyeContact.intruder = nullptr;
            guard->resetBinded();
            sendEvent(intruderArrested);
        }

        window->clear(backgroundColor);

        window->draw(area);
        window->draw(arrestArea);
        window->draw(guardRoom);
        guard->move();
        if (intruder != nullptr) {
            window->draw(intruder->getBody());
            intruder->move();
        }
        guard->drawSelf(window);

        if (noiseState != 0) {
            window->draw(*noiseEffect);
            noiseState = (noiseState + 1) % 12;
        }

        window->display();
        std::this_thread::sleep_until(startTime + dt);
    }

    return 0;
}