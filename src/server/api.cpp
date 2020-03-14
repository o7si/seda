#include "api.h"

#include <iostream>

namespace o7si
{
namespace server
{
    

// ----------------------------------------------------------------------------

bool help(const std::string& in, std::string& out)
{
    out = "1 + 1 = 2";
    return true;      
}

bool repeater(const std::string& in, std::string& out)
{
    out = in;
    return true;    
}

// ----------------------------------------------------------------------------

REGISTER_API(/repeater, repeater)
REGISTER_API(/help, help)

// ----------------------------------------------------------------------------


}   // namespace server    
}   // namespace o7si
