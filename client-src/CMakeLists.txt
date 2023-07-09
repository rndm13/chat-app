project(client)

add_executable(client main.cpp)

add_library(view view.hpp view-boilerplate.cpp view.cpp)

target_link_libraries(view 
    common_lib
    fmt::fmt
    IMGUI
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    -fsanitize=address # Debug only
    )


target_link_libraries(client
    view
    common_lib
    fmt::fmt
    IMGUI
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    -fsanitize=address # Debug only
    )

