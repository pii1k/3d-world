#include "application_builder.hpp"

int main()
{
    ads::assembly::ApplicationBuilder builder;
    auto app = builder.build();
    app.controller_ptr->runDemo();
    return 0;
}
