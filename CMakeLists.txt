project(Babel)
cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 17)
if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
endif()

add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)
