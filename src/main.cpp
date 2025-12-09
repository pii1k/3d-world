#include "application_builder.hpp"

int main()
{
    ads::assembly::ApplicationBuilder builder;
    auto app = builder.build();
    if (!app.simulation_runner_ptr)
        app.simulation_runner_ptr->run();
    return 0;
}