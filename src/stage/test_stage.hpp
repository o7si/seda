#pragma once

#include "../pch.h"

namespace o7si
{
namespace seda
{

class StageOne : public Stage
{
public:
    StageOne(std::string name, size_t capacity = 10)
        : Stage(std::move(name), capacity)
    {
    }

    std::pair<std::string, boost::any> handler(boost::any& args)
    {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        int count = boost::any_cast<int>(args);
        
        if (rand() & 1)
            return { "success", std::string(count, 'a')};
        return { "failure", std::string(count, 'b')};
    }
};

}   // namespace seda    
}   // namespace o7si
