#include <iostream>

#include "d2s/D2Character.hpp"
#include "d2s/Quest.hpp"


int main()
{
    Quest quest {
        "den of evil", 1, {
            "entered the den",
            "cleared the den"
        }
    };

    // for (size_t i=0; i<0; ++i) {
    //     quest.advanceState();
    // }

    // std::cout << quest.currentState() << "\n";

    auto const& states = quest.states();

    for (auto const& state : states) {
        std::cout << state << "\n";
    }

    return 0;
}
