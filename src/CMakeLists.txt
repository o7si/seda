aux_source_directory(. SRC)

add_executable(seda ${SRC}
    seda/stage.h seda/stage.cpp)

target_link_libraries(seda yaml-cpp)
