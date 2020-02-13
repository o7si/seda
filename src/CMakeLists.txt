aux_source_directory(. SRC)

add_executable(seda 
    seda/stage.h seda/stage.cpp
    stage/TestStage.hpp
    ${SRC}
)

find_package(Boost REQUIRED)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "not find Boost!")
endif ()

target_link_libraries(seda ${Boost_LIBRARIES})
target_link_libraries(seda yaml-cpp)
