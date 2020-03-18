/******************************************************************************
 * File: random.h
 * Description: 随机库。
 * Author: o7si
 *****************************************************************************/
#pragma once

#include <algorithm>
#include <random>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>


namespace o7si
{
namespace random
{

// ----------------------------------------------------------------------------

static const std::string Letters = 
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

static const std::string Lower_Letters =
    "abcdefghijklmnopqrstuvwxyz";

static const std::string Upper_Letters =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

static const std::string Digits =
    "0123456789";

// ----------------------------------------------------------------------------
    
// 随机数引擎
static std::default_random_engine generator; 

// 生成 UUID
// UUID 不受 seed 的影响
std::string uuid();

// 设置随机种子
void seed(int number);

// 生成 [left, right) 的一个整数型随机数
int randint(int left, int right);

// 生成 [0, 1) 的一个浮点型随机数
double random();

// 生成 [left, right) 的一个浮点型随机值
double uniform(double left, double right);

// 从序列中随机选取一个元素
template<typename T>
typename T::value_type choice(const T& container)
{
    size_t size = container.cend() - container.cbegin();
    size_t rand = randint(0, size);

    auto iter = container.cbegin();
    // 某些容器可能不支持随机访问迭代器
    while (rand --)
        iter ++;

    return *iter;
}

// 打乱序列中的元素（原序列将被打乱）
template<typename T>
void shuffle(T& container)
{
    std::shuffle(container.begin(), container.end(), generator); 
}

// 打乱序列中的元素（原序列不会被打乱） 
template<typename T>
T shuffle_copy(const T& container)
{
    T copy_container;
    std::copy(container.cbegin(), container.cend(), 
              std::back_inserter(copy_container));
    std::shuffle(copy_container.begin(), copy_container.end(), generator);
    return copy_container;
}

// ----------------------------------------------------------------------------

}   // namespace random
}   // namespace o7si
