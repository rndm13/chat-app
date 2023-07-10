project(server)

add_library(tcp_connection tcp_connection.hpp tcp_connection.cpp)

target_link_libraries(tcp_connection
    common_lib 
    fmt::fmt
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    -fsanitize=address # Debug only
    )

add_library(tcp_server tcp_server.hpp tcp_server.cpp)

target_link_libraries(tcp_server
    tcp_connection
    common_lib 
    fmt::fmt
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    -fsanitize=address # Debug only
    )

add_executable(server main.cpp)

target_link_libraries(server
    tcp_server
    tcp_connection
    common_lib 
    fmt::fmt
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    -fsanitize=address # Debug only
    )
