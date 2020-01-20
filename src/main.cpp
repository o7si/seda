#include <iostream>
#include "log.h"

int main(int argc, char* argv[])
{
#define LOGLEVEL_TEST(level) \
    o7si::LogLevel::ToString(o7si::LogLevel::FromString(#level))

    std::cout << LOGLEVEL_TEST(ABC) << std::endl;
    std::cout << LOGLEVEL_TEST(ALL) << std::endl;
    std::cout << LOGLEVEL_TEST(DEBUG) << std::endl;
    std::cout << LOGLEVEL_TEST(INFO) << std::endl;
    std::cout << LOGLEVEL_TEST(WARN) << std::endl;
    std::cout << LOGLEVEL_TEST(ERROR) << std::endl;
    std::cout << LOGLEVEL_TEST(FATAL) << std::endl;
    std::cout << LOGLEVEL_TEST(OFF) << std::endl;

    return 0;
}