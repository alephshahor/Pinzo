cmake_minimum_required(VERSION 3.5)

project(Pinzo LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)

set(INCLUDE_FILES mainwindow.h)
set(SOURCE_FILES main.cpp mainwindow.cpp)
set(UI_FILES mainwindow.ui)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(Pinzo
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
    ${UI_FILES}
)

target_link_libraries(Pinzo PRIVATE Qt5::Widgets)
