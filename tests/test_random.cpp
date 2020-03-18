#include <iostream>

#include <random.h>


void test_uuid()
{
    std::cout << __func__ << std::endl;
    for (size_t i = 0; i < 10; ++ i)
        std::cout << o7si::random::uuid() << std::endl;    
}

void test_randint()
{
    std::cout << __func__ << std::endl;
    for (size_t i = 0; i < 10; ++ i)
        std::cout << o7si::random::randint(0, 5) << " ";
    std::cout << std::endl;
}

void test_random()
{
    std::cout << __func__ << std::endl;
    for (size_t i = 0; i < 10; ++ i)
        std::cout << o7si::random::random() << " ";
    std::cout << std::endl;
}

void test_uniform()
{
    std::cout << __func__ << std::endl;
    for (size_t i = 0; i < 10; ++ i)
        std::cout << o7si::random::uniform(2.71, 3.14) << " ";
    std::cout << std::endl;
}

void test_choice()
{
    std::cout << __func__ << std::endl;
    for (size_t i = 0; i < 10; ++ i)
        std::cout << o7si::random::choice(o7si::random::Letters) << " ";
    std::cout << std::endl;
}

void test_shuffle()
{
    std::cout << __func__ << std::endl;
    for (size_t i = 0; i < 10; ++ i)
    {
        std::string seq = o7si::random::Letters;
        o7si::random::shuffle(seq);
        std::cout << seq << std::endl;
    }
}

void test_shuffle_copy()
{
    std::cout << __func__ << std::endl;
    for (size_t i = 0; i < 10; ++ i)
        std::cout << o7si::random::shuffle_copy(o7si::random::Letters) 
                  << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Please input seed..." << std::endl;
        std::cout << "e.g." << std::endl;
        std::cout << argv[0] << " 1000" << std::endl; 
        return EXIT_FAILURE;
    }

    test_uuid();
    test_randint();
    test_random();
    test_uniform();
    test_choice();
    test_shuffle();
    test_shuffle_copy();

    return EXIT_FAILURE;    
}
