#include "random.h"

namespace o7si
{
namespace random
{
    

std::string uuid()
{
    boost::uuids::uuid r = boost::uuids::random_generator()();
    return boost::uuids::to_string(r);
}


}   // namespace random
}   // namespace o7si
