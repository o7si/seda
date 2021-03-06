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


}   // namespace utils
}   // namespace o7si
