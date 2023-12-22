#include <iostream>

int sample_D2SaveFile(int const argc, char const* const* argv);
int splitStash(int const argc, char const* const* argv);

int main(int const argc, char const* const* argv)
{
    splitStash(argc, argv);
}


// #include "d2s/D2Character.hpp"
// #include "d2s/Quest.hpp"

// int main()
// {
//     Quest quest {
//         "den of evil", 1, {
//             "entered the den",
//             "cleared the den"
//         }
//     };

//     // for (size_t i=0; i<0; ++i) {
//     //     quest.advanceState();
//     // }

//     // std::cout << quest.currentState() << "\n";

//     auto const& states = quest.states();

//     for (auto const& state : states) {
//         std::cout << state << "\n";
//     }

//     return 0;
// }
