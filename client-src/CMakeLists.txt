project(client)

add_executable(client main.cpp)

add_library(view view.hpp view-boilerplate.cpp view.cpp)
add_library(controller controller.hpp controller.cpp)
add_library(model model.hpp model.cpp)

target_link_libraries(model
    common_lib
    fmt::fmt
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    -fsanitize=address # Debug only
    )

target_link_libraries(controller
    model
    common_lib
    fmt::fmt
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    -fsanitize=address # Debug only
    )

target_link_libraries(view 
    controller
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

