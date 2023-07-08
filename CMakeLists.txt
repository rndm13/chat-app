cmake_minimum_required(VERSION 3.15.0)

enable_language(C)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -pedantic -Wextra -Wnon-virtual-dtor -Wshadow -fno-omit-frame-pointer")

# ------- fmt --------

include(FetchContent)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)

FetchContent_MakeAvailable(fmt)

# ------ boost     --------

find_package(Threads REQUIRED)
find_package(Boost REQUIRED headers thread)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  MESSAGE( STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
  MESSAGE( STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}.")
  MESSAGE( STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}.")
endif()

# ------ client -------

project(client)

add_executable(client ./client-src/main.cpp)

target_link_libraries(client
  fmt::fmt
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES}
  -fsanitize=address # Debug only
  )

# ------ server -------

project(server)

add_executable(server ./server-src/main.cpp)

target_link_libraries(server
  fmt::fmt
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES}
  -fsanitize=address # Debug only
  )
