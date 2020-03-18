#include <seda/stage.h>

namespace o7si
{
namespace seda
{

#define STAGE(stage, time) \
class stage : public Stage \
{ \
public: \
    explicit stage(std::string name) \
        : Stage(std::move(name)) \
    { \
    } \
    std::pair<std::string, boost::any> handler(boost::any& args) override \
    { \
        int level = boost::any_cast<int>(args); \
        std::this_thread::sleep_for(std::chrono::milliseconds(time)); \
        return { "success", level + 1 }; \
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

}   // namespace seda
}   // namespace o7si
