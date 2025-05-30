#include "state_manager.h"

#include "player.h"

template<typename E>
bool StateManager<E>::transition(E state)
{
    m_previous_state = m_state;
    m_state = state;
    return true;
}

template class StateManager<PlayerState>;