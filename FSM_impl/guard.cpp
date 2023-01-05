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
// TODO: прикрутить изменение направления патрулирования
    void react(NewTick const & e) override {
        if (e.guard->getState() == State::DONE) {
            if (e.guard->getPatrolCount() % 2 == 0) {
                e.guard->setState(State::IN_PROGRESS);
                transit<CamerasCheck>();
            } else {
                e.guard->incrementPatrolCount();
                e.guard->setState(State::IN_PROGRESS);
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

FSM_INITIAL_STATE(GuardFSM, Patrolling)