#include "engine/engine.hpp"

int main()
{
    Engine engine;
    if (!engine.init())
        return -1;

    engine.run();
    return 0;
}