#include "application_builder.hpp"

int main()
{
    sim::assembly::ApplicationBuilder builder;
    auto app = builder.build();
    app.controller->runDemo(30, 0.5F);
    return 0;
}
