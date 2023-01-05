#include <tinyfsm.hpp>

#include "guard.hpp"
#include "fsmlist.hpp"

#include <iostream>

class Examination;
class Chase;
class CamerasCheck;
class Arrest;

// ----------------------------------------------------------------------------
// State: Patrolling
//

class Patrolling : public GuardFSM {
    void react(NextPatrolSide const & e) override {
        switch (e.guard->getPatrolSide()) {
            case 0: {
                e.guard->setDestination(sf::Vector2f(150, 150));
                break;
            }
            case 1: {
                e.guard->setDestination(sf::Vector2f(650, 150));
                break;
            }
            case 2: {
                e.guard->setDestination(sf::Vector2f(650, 650));
                break;
            }
            case 3: {
                e.guard->setDestination(sf::Vector2f(150, 650));
                e.guard->incrementPatrolCount();
                e.guard->setState(State::DONE);
                break;
            }
        }
        e.guard->incrementPatrolSide();
    };

    void react(NewTick const & e) override {
        if (e.guard->getState() == State::DONE) {
            if (e.guard->getPatrolCount() == 1) {
                e.guard->setDestination(sf::Vector2f(50, 650));
                transit<CamerasCheck>();
            }
        }
    };

    void react(EyeContact const & e) override {
        e.guard->setTarget(e.intruder->getBody());
        e.guard->setState(State::IN_PROGRESS);
        e.guard->setSpeed(200);
        transit<Chase>();
    }

    void react(Noise const & e) override {
        e.guard->setDestination(e.location);
        e.guard->setState(State::IN_PROGRESS);
        transit<Examination>();
    };
};


// ----------------------------------------------------------------------------
// State: Chase
//

class Chase : public GuardFSM {
    void react(NewTick const & e) override {
        if (e.guard->getState() == State::DONE) {
            e.guard->setState(State::IN_PROGRESS);
            e.guard->setSpeed(100);
            transit<Arrest>();
        } else {
            auto distance = e.guard->getBody().getPosition() - e.guard->getTarget()->getPosition();
            auto absDistance = std::sqrt(distance.x * distance.x + distance.y * distance.y);
            if (absDistance < e.guard->getSize() / 2) {
                e.guard->resetTarget();
                e.guard->setDestinationArrest();
                e.guard->setState(State::DONE);
            }
        }
    };
};


// ----------------------------------------------------------------------------
// State: Arrest
//

class Arrest : public GuardFSM {

};


// ----------------------------------------------------------------------------
// State: Examination
//

class Examination : public GuardFSM {

};

// ----------------------------------------------------------------------------
// State: Examination
//

class CamerasCheck : public GuardFSM {
    void react(CamerasChecked const & e) override {
        e.guard->setState(State::IN_PROGRESS);
        if (e.intruder != nullptr) {
            e.guard->setTarget(e.intruder->getBody());
            transit<Chase>();
        } else {
            e.guard->setDestination(sf::Vector2f(150, 650));
            transit<Patrolling>();
        }
    };
};

// ----------------------------------------------------------------------------
// Base state: default implementations
//

void GuardFSM::react(const EyeContact &) {
    std::cout << "Eye contact!" << std::endl;
}

void GuardFSM::react(const Noise &) {
    std::cout << "I heard something..." << std::endl;
}

void GuardFSM::react(const NewTick &) { }

void GuardFSM::react(const NextPatrolSide &) { }

void GuardFSM::react(const CamerasChecked &) {
    std::cout << "I've checked all cameras" << std::endl;
}

FSM_INITIAL_STATE(GuardFSM, Patrolling)