#pragma once

#include "../pch.h"
#include "../log.h"
#include "../seda/stage.h"

namespace o7si
{
namespace seda
{

class CountStage : public Stage
{
public:
    CountStage(std::string name, size_t max_thread)
        : Stage(std::move(name), max_thread) 
    {
    }
};

class MaxMinStage : public Stage
{
public:
    MaxMinStage(std::string name, size_t max_thread)
        : Stage(std::move(name), max_thread) 
    {
    }
};

class MultiStage : public Stage
{
public:
    MultiStage(std::string name, size_t max_thread)
        : Stage(std::move(name), max_thread) 
    {
    }
};

class BinaryStage : public Stage
{
public:
    BinaryStage(std::string name, size_t max_thread)
        : Stage(std::move(name), max_thread) 
    {
    }
};

}   // namespace seda    
}   // namespace o7si
