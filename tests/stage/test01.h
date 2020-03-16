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
        LOG_DEBUG(my-log-1) << "o7si"; \
        LOG_DEBUG(var) << level; \
        LOG_DEBUG(test) << #stage << ".begin"; \
        std::this_thread::sleep_for(std::chrono::milliseconds(time)); \
        LOG_DEBUG(test) << #stage << ".end"; \
        return { "success", level + 1 }; \
    } \
}; \
REGISTER_STAGE(stage)

STAGE(Stage1, 1500)
STAGE(Stage2, 2500)
STAGE(Stage3, 500)
STAGE(Stage4, 3500)
STAGE(Stage5, 3500)
STAGE(Stage6, 3500)
STAGE(Stage7, 3500)
STAGE(Stage8, 3500)
STAGE(Stage9, 3500)
STAGE(Stage10, 3500)
STAGE(Stage11, 3500)

}   // namespace seda
}   // namespace o7si
