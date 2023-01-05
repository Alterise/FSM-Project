#include <tinyfsm.hpp>

#include "guard.hpp"

using FsmList = tinyfsm::FsmList<GuardFSM>;

template<typename E>
void sendEvent(E const & event)
{
    FsmList::template dispatch<E>(event);
}