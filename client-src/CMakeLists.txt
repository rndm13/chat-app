project(client)

add_executable(client main.cpp)

target_link_libraries(client
    common_lib
    fmt::fmt
    IMGUI
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    -fsanitize=address # Debug only
    )

