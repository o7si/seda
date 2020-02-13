#pragma once

#include "../pch.h"
#include "../log.h"
#include "../seda/stage.h"

namespace o7si
{
namespace seda
{

/*    
stage:
    CountStage: 1
        Success: MaxMinStage
        Failure: CountStage
        Waiting: CountStage
    MaxMinStage: 3
        Success: MultiStage
        Failure: MaxMinStage
        Waiting: MaxMinStage
    MultiStage: 5
        Success: BinaryStage
        Failure: MultiStage
        Waiting: MultiStage
    BinaryStage: 7
*/

class CountStage : public Stage
{
public:
    CountStage(std::string name, size_t max_thread)
        : Stage(std::move(name), max_thread) 
    {
    }
    
    void call(std::vector<boost::any>&& args) override
    {
        LOG_DEBUG << m_name << ".call";    
    }  
};

class MaxMinStage : public Stage
{
public:
    MaxMinStage(std::string name, size_t max_thread)
        : Stage(std::move(name), max_thread) 
    {
    }
    
    void call(std::vector<boost::any>&& args) override
    {
        LOG_DEBUG << m_name << ".call";    
    }  
};

class MultiStage : public Stage
{
public:
    MultiStage(std::string name, size_t max_thread)
        : Stage(std::move(name), max_thread) 
    {
    }
    
    void call(std::vector<boost::any>&& args) override
    {
        LOG_DEBUG << m_name << ".call";    
    }  
};

class BinaryStage : public Stage
{
public:
    BinaryStage(std::string name, size_t max_thread)
        : Stage(std::move(name), max_thread) 
    {
    }
    
    void call(std::vector<boost::any>&& args) override
    {
        LOG_DEBUG << m_name << ".call";    
    }  
};

}   // namespace seda    
}   // namespace o7si
