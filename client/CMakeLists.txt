find_package(Qt5 COMPONENTS Widgets Network REQUIRED)
find_package(Boost COMPONENTS serialization REQUIRED)
find_package(OpenSSL COMPONENTS Crypto REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(PortAudio REQUIRED IMPORTED_TARGET portaudio-2.0)
pkg_check_modules(Opus REQUIRED IMPORTED_TARGET opus)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "ui")
set(CMAKE_AUTORCC ON)

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS include/*.hpp)

if (NOT MSVC)
    set (ASSETS "assets/SVG_Files/assets.qrc")
else()
    set (ASSETS "assets/PNG_Files/assets.qrc")
endif()

add_executable(babel_client ${SOURCES} ${HEADERS} ${ASSETS})
target_include_directories(babel_client PUBLIC include)
target_link_libraries(babel_client babel_common PkgConfig::Opus PkgConfig::PortAudio OpenSSL::Crypto Qt5::Widgets Qt5::Network ${Boost_LIBRARIES})
