#include <server/web_server.h>

int main(int argc, char* argv[])
{
    o7si::server::WebServer server(4, atoi(argv[1]), "/root/reps/seda/web");
    server.start();
    std::this_thread::sleep_for(std::chrono::seconds(2));    
    server.stop();

    server.setProtocol(6);
    server.restart();
    std::this_thread::sleep_for(std::chrono::seconds(2000));    
    return 0;    
}
