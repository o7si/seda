#include "utils.h"

namespace o7si
{
namespace utils
{


time_t get_timestamp()
{
    return std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now()
    );    
}

uint64_t get_thread_id()
{
    // C
    return pthread_self();    
    // C++
    // return std::this_thread::get_id();
}

void set_thread_name(const std::string& name)
{
    pthread_setname_np(
        get_thread_id(), 
        name.substr(0, THREAD_NAME_MAX_LENGTH - 1).data()
    );    
}

std::string get_thread_name()
{
    char buffer[THREAD_NAME_MAX_LENGTH];
    pthread_getname_np(
        get_thread_id(),
        buffer,
        THREAD_NAME_MAX_LENGTH
    );    
    return std::string(buffer);
}

std::string to_upper(const std::string& str)
{
    std::string ret;
    ret.resize(str.size());
    std::transform(str.cbegin(), str.cend(), ret.begin(), ::toupper); 
    return ret;
}

std::string to_lower(const std::string& str)
{
    std::string ret;
    ret.resize(str.size());
    std::transform(str.cbegin(), str.cend(), ret.begin(), ::tolower); 
    return ret;
}

bool load_file(const std::string& filename, std::string& content)
{
    try
    {
        std::ifstream stream(filename, std::ifstream::in);

        // 无法打开文件
        if (!stream.is_open())
        {
            content = "";
            return false;
        }

        content = {
            std::istreambuf_iterator<char>(stream),
            std::istreambuf_iterator<char>(), 
        };
        stream.close();
    }
    catch (...)
    {
        // 读取文件时出现异常
        content = "";
        return false; 
    }

    // 文件正常打开且读取内容时无异常
    return true;
}

}   // namespace utils
}   // namespace o7si
