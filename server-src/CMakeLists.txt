project(server)

add_executable(server main.cpp)

target_link_libraries(server
    common_lib 
    fmt::fmt
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    -fsanitize=address # Debug only
    )
