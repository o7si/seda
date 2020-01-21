#include <iostream>
#include <chrono>
#include <thread>
#include "log.h"

int main(int argc, char* argv[])
{

    o7si::log::Logger::getInstance()->setAppenders(
            {
                    std::shared_ptr<o7si::log::Appender>(new o7si::log::ConsoleAppender),
                    std::shared_ptr<o7si::log::Appender>(new o7si::log::FileAppender("log.txt"))
            }
    );
    o7si::log::Event(o7si::log::Logger::getInstance(), o7si::log::Level::DEBUG,
                     o7si::log::Event::Information(
                             0, 0, "", __FILE__, __FUNCTION__, __LINE__
                     )).stream() << "Begin" << "!!!";

    int count = 0;
    while (count ++ < 10)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    o7si::log::Event(o7si::log::Logger::getInstance(), o7si::log::Level::INFO,
                     o7si::log::Event::Information(
                             0, 0, "", __FILE__, __FUNCTION__, __LINE__
                     )).stream() << "End" << "!!!";
    return 0;
};