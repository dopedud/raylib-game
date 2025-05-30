#pragma once

#include <type_traits>

template<typename E>
class StateManager
{
    static_assert(std::is_enum_v<E>, "StateManager only works with enumeration types.");

    E m_state {};
    E m_previous_state {};

public:
    explicit StateManager() {}
    explicit StateManager(E state) : m_state { state }, m_previous_state { state } {}
    
    E state() const { return m_state; }
    E previous_state() const { return m_previous_state; }

    void set_state(E state)
    {
        m_previous_state = m_state;
        m_state = state;
    }
    
    bool is_state(E state) const { return m_state == state; }
    bool transition(E state);
};