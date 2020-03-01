#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <string>
#include <vector>
#include <array>
#include <stack>
#include <queue>
#include <map>
#include <unordered_map>

#include <functional>

#include <memory>

#include <thread>
#include <mutex>
#include <future>
#include <chrono>
#include <condition_variable>

#include <sys/syscall.h>

#include <net/analysis/http11_parser.h>
#include <net/analysis/httpclient_parser.h>

#include <yaml-cpp/yaml.h>
#include <boost/any.hpp>


#define NAMESPACE_SEDA o7si::seda
#define NAMESPACE_LOG o7si::log
#define NAMESPACE_CONFIG o7si::config

namespace o7si
{

namespace seda {}
namespace log {}
namespace config {}

}
