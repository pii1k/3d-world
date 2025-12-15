#include <iostream>

#include "engine.hpp"

int main()
{
    try
    {
        Engine engine;
        engine.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "An error occured: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}