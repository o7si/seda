#include <iostream>
#include <random.h>


void test_uuid()
{
    for (int i = 0; i < 20; ++ i)
        std::cout << o7si::random::uuid() << std::endl;    
}

int main(int argc, char* argv[])
{
    test_uuid();
    return 0;    
}
