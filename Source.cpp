#include "first_app.hpp"
#include <exception>
#include <iostream>

using namespace myve;

int main()
{
    FirstApp app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

}