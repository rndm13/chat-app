add_library(common_lib common_lib.hpp common_lib.cpp)

target_link_libraries(common_lib
    ${CMAKE_THREAD_LIBS_INIT}
    fmt::fmt
    ${Boost_LIBRARIES}
    -fsanitize=address # Debug only
    )
