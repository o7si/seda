#include <oseda/seda.h>

#define STAGE(stage, time) \
class stage : public o7si::seda::Stage \
{ \
public: \
    explicit stage(std::string name) \
        : Stage(std::move(name)) \
    { \
    } \
    std::pair<std::string, boost::any> handler(boost::any& args) override \
    { \
        std::this_thread::sleep_for(std::chrono::milliseconds(time)); \
        return std::make_pair("success", 0); \
    } \
}; \
REGISTER_STAGE(stage)

STAGE(Stage1, 10)
STAGE(Stage2, 15)
STAGE(Stage3, 13)
STAGE(Stage4, 32)
STAGE(Stage5, 35)
STAGE(Stage6, 29)
STAGE(Stage7, 20)
STAGE(Stage8, 23)
STAGE(Stage9, 18)
STAGE(Stage10, 7)


int main(int argc, char* argv[])
{
    o7si::config::load("/root/reps/seda/conf/test.conf");
    o7si::server::WebServer server;
    server.start();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    auto stage1 = o7si::seda::StageManager::Instance()->doLogin("Stage1");
    while (true)
    {
        stage1->call(0);        
        stage1->call(0);        
        stage1->call(0);        

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;    
}
