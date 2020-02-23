cmake_minimum_required(VERSION 3.0)

project(seda)

set(SRCS
    src/pch.h
    src/log.h
    src/log.cpp
    src/config.h 
    src/config.cpp
    src/utils.h
    src/utils.cpp
    src/net/http.h
    src/net/http.cpp
    src/seda/macro.hpp
    src/seda/event_queue.hpp     
    src/seda/event_handler.hpp
    src/seda/performeter.hpp
    src/seda/thread_pool.hpp
    src/seda/stage.h
    src/seda/stage.cpp
    src/seda/stage_manager.h
    src/seda/stage_manager.cpp
)

add_library(seda SHARED ${SRCS})

set(LIBS
    seda
    pthread
    yaml-cpp
)

include_directories(src)

add_executable(test_seda test/test_seda.cpp test/stage/test_stage.hpp)
target_link_libraries(test_seda ${LIBS})
add_executable(test_net test/test_net.cpp)
target_link_libraries(test_net ${LIBS})
