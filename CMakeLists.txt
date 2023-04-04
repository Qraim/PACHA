cmake_minimum_required(VERSION 3.25)
project(PACHA2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(PACHA2 main.cpp MainWidow.cpp MainWidow.h)
target_link_libraries(PACHA2
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

