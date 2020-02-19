#include "utils.h"

namespace o7si
{
namespace utils
{

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
}   // namespace utils
}   // namespace o7si
