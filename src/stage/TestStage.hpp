#pragma once

#include "../pch.h"
#include "../log.h"
#include "../seda/stage.h"

namespace o7si
{
namespace seda
{

void myfunc(const std::unordered_map<std::string, boost::any>& argc, std::promise<std::string> promise)
{
    std::cout << __func__ << std::endl;    
}

class CountStage : public Stage
{
public:
    CountStage(std::string name, size_t max_thread)
        : Stage(std::move(name), max_thread)
    {
    }

    virtual void handle(const std::unordered_map<std::string, boost::any>& args, std::promise<std::string> promise) override
    {
        std::cout << "CountStage.Handle" << std::endl;    
    }  
    void handle2(const Args& args, std::promise<std::string> promise) 
    {
        std::cout << "CountStage.Handle" << std::endl;    
    }  
};

class MaxMinStage : public Stage
{
public:
    MaxMinStage(std::string name, size_t max_thread)
        : Stage(std::move(name), max_thread)
    {
    }

    virtual void handle(const Args& args, std::promise<std::string> promise) override
    {
        std::cout << "MaxMinStage.Handle" << std::endl;    
    }  
};

class MultiStage : public Stage
{
public:
    MultiStage(std::string name, size_t max_thread)
        : Stage(std::move(name), max_thread)
    {
    }
    virtual void handle(const Args& args, std::promise<std::string> promise) override
    {
        std::cout << "MultiStage.Handle" << std::endl;    
    }  
};

class BinaryStage : public Stage
{
public:
    BinaryStage(std::string name, size_t max_thread)
        : Stage(std::move(name), max_thread)
    {
    }
    virtual void handle(const Args& args, std::promise<std::string> promise) override
    {
        std::cout << "BinaryStage.Handle" << std::endl;    
    }  
};

}   // namespace seda    
}   // namespace o7si
