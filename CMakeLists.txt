cmake_minimum_required(VERSION 3.0)

project(seda)

add_compile_options(-g)

# HTTP 协议解析源码
set(HTTP_ANALYSIS_SRCS
    src/net/analysis/http11_common.h
    src/net/analysis/http11_parser.h
    src/net/analysis/http11_parser.cpp
    src/net/analysis/httpclient_parser.h
    src/net/analysis/httpclient_parser.cpp
)
add_library(http-analysis SHARED ${HTTP_ANALYSIS_SRCS})

set(SRCS
    src/log.h
    src/log.cpp
    src/config.h 
    src/config.cpp
    src/utils.h
    src/utils.cpp
    src/net/http.h
    src/net/http.cpp
    src/net/http_parser.h
    src/net/http_parser.cpp
    src/seda/macro.h
    src/seda/stage.h
    src/seda/stage.cpp
    src/seda/stage_manager.h
    src/seda/stage_manager.cpp
    src/seda/component/event_queue.hpp     
    src/seda/component/event_handler.hpp
    src/seda/component/performeter.hpp
    src/seda/component/thread_pool.hpp
)

add_library(seda SHARED ${SRCS})

set(LIBS
    seda
    pthread
    yaml-cpp
    http-analysis
)

include_directories(src)

add_executable(test_log tests/test_log.cpp)
target_link_libraries(test_log ${LIBS})
add_executable(test_config tests/test_config.cpp)
target_link_libraries(test_config ${LIBS})
add_executable(test_stage tests/test_stage.cpp)
target_link_libraries(test_stage ${LIBS})
