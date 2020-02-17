#pragma once

#include "../pch.h"

namespace o7si
{
namespace seda
{

class StageOne : public Stage
{
public:
    StageOne(std::string name, size_t capacity = 1)
        : Stage(std::move(name), capacity)
    {
    }

    boost::any handler(boost::any&& args)
    {
        int count = boost::any_cast<int>(args);
        return std::string(count, 'a');    
    }
};

}   // namespace seda    
}   // namespace o7si
