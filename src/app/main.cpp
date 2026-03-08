#include "engine.hpp"
#include "game.hpp"
#include <memory>

// This function will be implemented by the specific game library (e.g., snake_game)
// and linked at compile time.
std::unique_ptr<domain::Game> create_game();

int main()
{
    auto game = create_game();
    engine::Engine engine(*game);
    if (!engine.init())
        return -1;

    engine.run();
    return 0;
}