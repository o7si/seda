aux_source_directory(. SRC)

add_executable(seda 
    seda/stage.h
    seda/stage.cpp
    seda/stage_manager.h
    seda/stage_manager.cpp
    seda/event_queue.hpp
    seda/thread_pool.hpp
    stage/test_stage.hpp
    ${SRC}
)

find_package(Boost REQUIRED)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "not find Boost!")
endif ()

target_link_libraries(seda ${Boost_LIBRARIES})
target_link_libraries(seda yaml-cpp)
target_link_libraries(seda pthread)
