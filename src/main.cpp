#include "application_builder.hpp"

int main()
{
    ads::assembly::ApplicationBuilder builder;
    auto app = builder.build();
    app.controller_ptr->runDemo(30, 0.5F);
    return 0;
}
