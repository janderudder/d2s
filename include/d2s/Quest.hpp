#ifndef QUEST_HPP_INCLUDED
#define QUEST_HPP_INCLUDED

#include <bitset>
#include <cstdint>
#include <span>
#include <string>
#include <vector>

class Quest
{
    std::string              m_name;
    unsigned int             m_actNumber;
    std::vector<std::string> m_states;
    size_t                   m_currentStateIndex;

    constexpr static size_t  MAX_STATES = 32;

public:
    constexpr static size_t NOT_STARTED = MAX_STATES;
    constexpr static size_t STARTED     = MAX_STATES + 1;
    constexpr static size_t COMPLETED   = MAX_STATES + 2;

public:
    Quest(
        std::string_view name,
        unsigned int actNumber,
        std::initializer_list<std::string> states,
        uint32_t currentStateIndex = 0
    ) noexcept;

    auto isCompleted() const -> bool;
    auto currentState() const -> std::string_view;
    auto states() const -> std::span<std::string const>;

    auto advanceState() -> std::string_view;

private:
    auto _lastStateIndex() const -> size_t;

};

#endif // QUEST_HPP_INCLUDED
