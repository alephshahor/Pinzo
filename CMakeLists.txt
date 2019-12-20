cmake_minimum_required(VERSION 3.5)

project(Pinzo LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/resources)


set(INCLUDE_FILES
    mainwindow.h
    include/image.h
    include/util.h
    include/qcustomplot.h
    include/rubberband.h
    include/geometricfunctions.h
    saveimagedialog.h
    histogram.h
    cumulativehistogram.h
    absolutehistogram.h
    imageadjuster.h
    lineartransformation.h
    histogramspecification.h
    gammacorrection.h
    imagedifference.h
    differencemap.h
    scaling.h
    rotation.h
    )

set(SOURCE_FILES
    main.cpp
    mainwindow.cpp
    src/image.cpp
    src/util.cpp
    src/qcustomplot.cpp
    src/rubberband.cpp
    src/geometricfunctions.cpp
    saveimagedialog.cpp
    histogram.cpp
    cumulativehistogram.cpp
    absolutehistogram.cpp
    imageadjuster.cpp
    lineartransformation.cpp
    histogramspecification.cpp
    gammacorrection.cpp
    imagedifference.cpp
    differencemap.cpp
    scaling.cpp
    rotation.cpp
    )

set(UI_FILES
    mainwindow.ui
    saveimagedialog.ui
    cumulativehistogram.ui
    absolutehistogram.ui
    histogram.ui
    imageadjuster.ui
    lineartransformation.ui
    histogramspecification.ui
    gammacorrection.ui
    imagedifference.ui
    differencemap.ui
    scaling.ui
    rotation.ui
    )

set(TEST_FILES
    test/catch.hpp
    )

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Charts REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS PrintSupport REQUIRED)


add_executable(Pinzo
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
    ${TEST_FILES}
    ${UI_FILES}
)

#add_executable(Test
#    ${INCLUDE_FILES}
#    ${SOURCE_FILES}
#    ${TEST_FILES}
#)

target_compile_definitions(Pinzo PUBLIC CURRENT_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(Pinzo PUBLIC ${CMAKE_INCLUDE_SOURCE_DIR}/include)

target_link_libraries(Pinzo PRIVATE Qt5::Widgets)
target_link_libraries(Pinzo PUBLIC Qt5::Charts)
target_link_libraries(Pinzo PUBLIC Qt5::Gui)
target_link_libraries(Pinzo PUBLIC Qt5::PrintSupport)

#target_compile_definitions(Test PUBLIC CURRENT_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}")
#target_include_directories(Test PUBLIC ${CMAKE_INCLUDE_SOURCE_DIR}/include)
#target_link_libraries(Test PRIVATE Qt5::Widgets)
#target_link_libraries(Test PUBLIC Qt5::Charts)
#target_link_libraries(Test PUBLIC Qt5::Gui)
#target_link_libraries(Test PUBLIC Qt5::PrintSupport)
