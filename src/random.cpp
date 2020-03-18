#include "random.h"

namespace o7si
{
namespace random
{
    
// ----------------------------------------------------------------------------

std::string uuid()
{
    boost::uuids::uuid r = boost::uuids::random_generator()();
    return boost::uuids::to_string(r);
}

void seed(int number)
{
    generator.seed(number);    
}

int randint(int left, int right)
{
    std::uniform_int_distribution<int> distribution(left, right - 1);
    return distribution(generator); 
}

double random()
{
    std::uniform_real_distribution<double> distribution(0, 1);    
    return distribution(generator);
}

double uniform(double left, double right)
{
    std::uniform_real_distribution<double> distribution(left, right);
    return distribution(generator);   
}

// ----------------------------------------------------------------------------

}   // namespace random
}   // namespace o7si
