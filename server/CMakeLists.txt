find_package(Boost COMPONENTS serialization program_options REQUIRED)
find_package(OpenSSL COMPONENTS SSL REQUIRED)

file(GLOB SOURCES src/*.cpp)

add_executable(babel_server ${SOURCES})
target_include_directories(babel_server PUBLIC include)
target_link_libraries(babel_server babel_common OpenSSL::SSL Boost::serialization Boost::program_options)
