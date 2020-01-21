#include "utils.h"

namespace o7si
{
namespace utils
{

uint64_t convertThreadId(std::thread::id threadId)
{
    std::stringstream stream;
    stream << threadId;
    uint64_t id;
    stream >> id;
    return id;
}

}   // namespace utils
}   // namespace o7si