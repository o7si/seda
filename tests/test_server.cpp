#include <server.h>

int main(int argc, char* argv[])
{
    using namespace o7si::server;
    WebServer server(4, atoi(argv[1]), "/root/reps/seda/web");
    server.start();
    std::cout << "sleep" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));    


    std::this_thread::sleep_for(std::chrono::seconds(2000));    
    return 0;    
}
