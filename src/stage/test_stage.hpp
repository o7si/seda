#pragma once

#include "../pch.h"
#include "../seda/macro.hpp"

namespace o7si
{
namespace seda
{
/*
 * 说明:
 *      1. CountStage: 接受一个字符串，统计每个字母出席的频率，返回一个 map
 *      2. MaxMinStage: 接受一个 map，寻找出现频率最高和最低的字母，返回两个字母的 ASCII 码
 *      3. MultiStage: 接受两个整数，返回两个整数的乘积
 *      4. BinaryStage: 接受一个整数，输出该数的二进制形式
 */
class CountStage : public Stage
{
public:
    explicit CountStage(std::string name)    
        : Stage(std::move(name))
    {
    }

    std::pair<std::string, boost::any> handler(boost::any& args) override
    {
        std::string seq = boost::any_cast<std::string>(args);
        std::map<char, int> cnt;
        
        for (int i = 0; i < seq.size(); ++ i)
            ++ cnt[seq[i]];    

        int random = rand() % 3;
        if (random == 0)
            return { "success", cnt };
        if (random == 1)
            return { "failure", args };
         return { "waiting", args };
    }
};

REGISTER_STAGE(CountStage)

class MaxMinStage : public Stage
{
public:
    explicit MaxMinStage(std::string name)    
        : Stage(std::move(name))
    {
    }

    std::pair<std::string, boost::any> handler(boost::any& args) override
    {
        std::map<char, int> cnt = boost::any_cast<std::map<char, int>>(args);
        
        int minNum = 1000, maxNum = 0;
        char minCh, maxCh;
        for (auto iter = cnt.begin(); iter != cnt.end(); ++ iter)
        {
            if (iter->second < minNum)
            {
                minNum = iter->second;
                minCh = iter->first;
            }
            if (iter->second > maxNum)
            {
                maxNum = iter->second;
                maxCh = iter->first;
            }
        }

        int random = rand() % 3;
        if (random == 0)
            return { "success", std::make_tuple((int)minCh, (int)maxCh) };
        if (random == 1)
            return { "failure", args };
         return { "waiting", args };
    }
};

REGISTER_STAGE(MaxMinStage)

class MultiStage : public Stage
{
public:
    explicit MultiStage(std::string name)
        : Stage(std::move(name)) 
    {
    }
    
    std::pair<std::string, boost::any> handler(boost::any& args) override
    {
        std::tuple<int, int> tuple = boost::any_cast<std::tuple<int, int>>(args);
        int a = std::get<0>(tuple);
        int b = std::get<1>(tuple);
        
        int c = a * b;
              
        int random = rand() % 3;
        if (random == 0)
            return { "success", c };
        if (random == 1)
            return { "failure", args };
        return { "waiting", args };
    }
};

REGISTER_STAGE(MultiStage)

class BinaryStage : public Stage
{
public:
    explicit BinaryStage(std::string name)
        : Stage(std::move(name))
    {
    }

    std::pair<std::string, boost::any> handler(boost::any& args) override
    {
        int multi = boost::any_cast<int>(args);
        
        std::stack<int> sta;
        while (multi != 0)
        {
            sta.push(multi % 2);
            multi /= 2;    
        }

        std::string res;
        while (!sta.empty())
        {
            res += std::to_string(sta.top());
            sta.pop();   
        }

        LOG_INFO << "res = " << res;
        return { "final", res };
    }
};

REGISTER_STAGE(BinaryStage)

}   // namespace seda    
}   // namespace o7si
