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
