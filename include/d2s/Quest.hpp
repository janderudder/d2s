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
    uint8_t                  m_actNumber;
    std::vector<std::string> m_states;
    uint8_t                  m_currentStateIndex;

public:
    enum : uint8_t { NOT_STARTED, STARTED, COMPLETED };

public:
    Quest(
        std::string_view name,
        uint8_t actNumber,
        std::initializer_list<std::string> states,
        uint8_t currentStateIndex = 0
    ) noexcept;

    auto actNumber() const -> uint8_t;
    auto isCompleted() const -> bool;
    auto currentState() const -> std::string_view;
    auto currentStateIndex() const -> uint8_t;
    auto states() const -> std::span<std::string const>;

    auto advanceState() -> std::string_view;

private:
    auto _lastStateIndex() const -> uint8_t;

};

#endif // QUEST_HPP_INCLUDED
