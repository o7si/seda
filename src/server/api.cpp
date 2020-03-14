#include "api.h"

#include <iostream>

namespace o7si
{
namespace server
{
    

bool repeater(const std::string& in, std::string& out)
{
    out = in;
    return true;    
}

REGISTER_API(/repeater, repeater)
    

}   // namespace server    
}   // namespace o7si
