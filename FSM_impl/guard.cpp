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
                e.guard->setState(State::GUARD_DONE);
                break;
            }
        }
        e.guard->incrementPatrolSide();
    };

    void react(NewTick const & e) override {
        if (e.guard->getState() == State::GUARD_DONE) {
            if (e.guard->getPatrolCount() == 1) {
                e.guard->setDestination(sf::Vector2f(50, 650));
                transit<CamerasCheck>();
            }
        }
    };

    void react(EyeContact const & e) override {
        e.guard->setTarget(e.intruder);
        e.guard->setState(State::GUARD_DONE);
        e.guard->setSpeed(200);
        e.guard->resetPatrol();
        transit<Chase>();
    }

    void react(Noise const & e) override {
        e.guard->setDestination(e.location);
        e.guard->setState(State::GUARD_IN_PROGRESS);
        transit<Examination>();
    };
};


// ----------------------------------------------------------------------------
// State: Chase
//

class Chase : public GuardFSM {
    void react(NewTick const & e) override {
        auto distance = e.guard->getBody().getPosition() - e.guard->getTarget()->getPosition();
        auto absDistance = std::sqrt(distance.x * distance.x + distance.y * distance.y);
        if (absDistance < e.guard->getSize() / 2) {
            e.guard->resetTarget();
            e.guard->setDestination(sf::Vector2f(750, 650));
            e.guard->setState(State::GUARD_IN_PROGRESS);
            e.guard->setSpeed(100);
            e.intruder->setState(State::INTRUDER_ARRESTING);
            e.guard->setBinded(e.intruder);
            transit<Arrest>();
        }
    };

    void react(IntruderLost const & e) override {
        e.guard->resetTarget();
        e.guard->setDestination(sf::Vector2f(150, 650));
        e.guard->setState(State::GUARD_IN_PROGRESS);
        e.guard->setSpeed(100);
        transit<Patrolling>();
    };
};


// ----------------------------------------------------------------------------
// State: Arrest
//

class Arrest : public GuardFSM {
    void react(IntruderArrested const & e) override {
        e.guard->setDestination(sf::Vector2f(150, 650));
        transit<Patrolling>();
    };
};


// ----------------------------------------------------------------------------
// State: Examination
//

class Examination : public GuardFSM {
    void react(EyeContact const & e) override {
        e.guard->setTarget(e.intruder);
        e.guard->setState(State::GUARD_DONE);
        e.guard->setSpeed(200);
        e.guard->resetPatrol();
        transit<Chase>();
    }

    void react(Noise const & e) override {
        e.guard->setDestination(e.location);
        e.guard->setState(State::GUARD_IN_PROGRESS);
    };

    void react(NoiseChecked const & e) override {
        e.guard->setDestination(sf::Vector2f(150, 650));
        e.noise->setPosition(-100, 100);
        transit<Patrolling>();
    };
};

// ----------------------------------------------------------------------------
// State: Examination
//

class CamerasCheck : public GuardFSM {
    void react(CamerasChecked const & e) override {
        e.guard->setState(State::GUARD_IN_PROGRESS);
        if (e.intruder != nullptr) {
            e.guard->setTarget(e.intruder);
            e.guard->setSpeed(200);
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

void GuardFSM::react(const EyeContact &) { }

void GuardFSM::react(const Noise &) { }

void GuardFSM::react(const NewTick &) { }

void GuardFSM::react(const NextPatrolSide &) { }

void GuardFSM::react(const CamerasChecked &) { }

void GuardFSM::react(const IntruderArrested &) { }

void GuardFSM::react(const IntruderLost &) { }

void GuardFSM::react(const NoiseChecked &) { }

FSM_INITIAL_STATE(GuardFSM, Patrolling)