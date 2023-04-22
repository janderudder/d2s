#include "d2s/Quest.hpp"


Quest::Quest(
    std::string_view name,
    uint8_t actNumber,
    std::initializer_list<std::string> states,
    uint8_t currentStateIndex
) noexcept:
    m_name              {name},
    m_actNumber         {actNumber}
{
    m_states.reserve(states.size() + 3);
    m_states.push_back("not started");
    m_states.push_back("started");
    m_states.insert(m_states.end(), states);
    m_states.push_back("completed");

    switch (currentStateIndex) {
        case NOT_STARTED:
            m_currentStateIndex = 0;
        break;
        case STARTED:
            m_currentStateIndex = 1;
        break;
        case COMPLETED:
            m_currentStateIndex = _lastStateIndex();
        break;
        default:
            m_currentStateIndex = currentStateIndex + 2;
    }
}



////////////////////////////////////////////////////////////////////////////////
// getters
////////////////////////////////////////////////////////////////////////////////
auto Quest::actNumber() const -> uint8_t
{
    return m_actNumber;
}



auto Quest::isCompleted() const -> bool
{
    return m_currentStateIndex == _lastStateIndex();
}



auto Quest::currentState() const -> std::string_view
{
    return m_states[m_currentStateIndex];
}



auto Quest::currentStateIndex() const -> uint8_t
{
    return m_currentStateIndex;
}



auto Quest::states() const -> std::span<std::string const>
{
    return {m_states};
}



////////////////////////////////////////////////////////////////////////////////
// setters
////////////////////////////////////////////////////////////////////////////////
auto Quest::advanceState() -> std::string_view
{
    if (m_currentStateIndex < _lastStateIndex()) {
        ++m_currentStateIndex;
    }
    return m_states[m_currentStateIndex];
}



////////////////////////////////////////////////////////////////////////////////
// private
////////////////////////////////////////////////////////////////////////////////
auto Quest::_lastStateIndex() const-> uint8_t
{
    return m_states.size() - 1;
}
