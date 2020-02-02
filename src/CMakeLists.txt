aux_source_directory(. SRC)

add_executable(seda ${SRC})

target_link_libraries(seda yaml-cpp)
