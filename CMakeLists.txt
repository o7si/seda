cmake_minimum_required(VERSION 3.0) 
project(seda)

# 使用 clang 编写项目
set(CMAKE_C_COMPILER clang)
set(CMAKE_CPP_COMPILER clang)

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
    src/config.cpp
    src/config.h 
    src/log.cpp
    src/log.h
    src/net/http.cpp
    src/net/http.h
    src/net/http_parser.cpp
    src/net/http_parser.h
    src/net/sockaddr.cpp
    src/net/sockaddr.h
    src/net/socket.cpp
    src/net/socket.h
    src/random.cpp
    src/random.h
    src/seda/component/event_handler.hpp
    src/seda/component/event_queue.hpp     
    src/seda/component/performeter.hpp
    src/seda/component/thread_pool.hpp
    src/seda/macro.h
    src/seda/stage.cpp
    src/seda/stage.h
    src/seda/stage_manager.cpp
    src/seda/stage_manager.h
    src/server/api.cpp
    src/server/api.h
    src/server/api_manager.cpp
    src/server/api_manager.h
    src/server/servlet.cpp
    src/server/servlet.h
    src/server/web_server.cpp
    src/server/web_server.h
    src/utils.cpp
    src/utils.h
)
add_library(seda SHARED ${SRCS})

set(LIBS
    http-analysis
    jsoncpp
    pthread
    seda
    yaml-cpp
)

include_directories(src)

#add_executable(test_log tests/test_log.cpp)
#target_link_libraries(test_log ${LIBS})
#add_executable(test_config tests/test_config.cpp)
#target_link_libraries(test_config ${LIBS})
#add_executable(test_stage tests/test_stage.cpp)
#target_link_libraries(test_stage ${LIBS})
#add_executable(test_net tests/test_net.cpp)
#target_link_libraries(test_net ${LIBS})
#add_executable(test_sockaddr tests/test_sockaddr.cpp)
#target_link_libraries(test_sockaddr ${LIBS})
#
#add_executable(test_socket_server tests/test_socket_server.cpp)
#target_link_libraries(test_socket_server ${LIBS})
#add_executable(test_socket_client tests/test_socket_client.cpp)
#target_link_libraries(test_socket_client ${LIBS})
#
#add_executable(test_socket_udp_server tests/test_socket_udp_server.cpp)
#target_link_libraries(test_socket_udp_server ${LIBS})
#add_executable(test_socket_udp_client tests/test_socket_udp_client.cpp)
#target_link_libraries(test_socket_udp_client ${LIBS})
#
#add_executable(test_sockaddr_print tests/test_sockaddr_print.cpp)
#target_link_libraries(test_sockaddr_print ${LIBS})

add_executable(test_server tests/test_server.cpp)
target_link_libraries(test_server ${LIBS})

add_executable(test_jsoncpp tests/test_jsoncpp.cpp)
target_link_libraries(test_jsoncpp ${LIBS})

add_executable(test_random tests/test_random.cpp)
target_link_libraries(test_random ${LIBS})
