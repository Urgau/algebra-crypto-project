find_package(Boost COMPONENTS serialization REQUIRED)

file(GLOB_RECURSE SOURCES ./src *.cpp)

add_library(babel_common ${SOURCES})
target_include_directories(babel_common PUBLIC include)
target_link_libraries(babel_common ${Boost_LIBRARIES})
