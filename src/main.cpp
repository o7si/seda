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
    )->setLevel(o7si::log::Level::DEBUG);

    LOG(DEBUG) << "DEBUG" << "!!!";
    LOG(INFO) << "INFO" << "!!!";
    LOG(WARN) << "WARN" << "!!!";
    LOG(ERROR) << "ERROR" << "!!!";
    LOG(FATAL) << "FATAL" << "!!!";
    return 0;
};