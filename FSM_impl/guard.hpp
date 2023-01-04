#pragma once

#include <tinyfsm.hpp>
#include "events.hpp"

class GuardFSM : public tinyfsm::Fsm<GuardFSM>
{
public:
    void react(tinyfsm::Event const &) { };

    virtual void react(EyeContact const &);

    virtual void entry(void) { };
    void         exit(void)  { };
};
