#include "Game.h"
#include <iostream>

int main()
{
    try
    {
        std::cout << "=== Car Dodge Game ===\n";
        std::cout << "Starting game...\n\n";

        Game game;
        game.run();

        std::cout << "\nThanks for playing Car Dodge!\n";
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}