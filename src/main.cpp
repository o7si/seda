#include <iostream>
#include <memory>
#include <thread>
#include "log.h"


int main(int argc, char* argv[])
{

    o7si::log::Logger::getInstance()->setAppenders(
            {
                    std::shared_ptr<o7si::log::Appender>(new o7si::log::ConsoleAppender(
                            std::make_shared<o7si::log::Layout>("")
                    )),
                    std::shared_ptr<o7si::log::Appender>(new o7si::log::FileAppender("log.txt",
                            std::make_shared<o7si::log::Layout>(""))
                    )
            }
    );

    int count = 10;
    while (count --)
    {
        LOG(DEBUG) << "count = " << count << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
};