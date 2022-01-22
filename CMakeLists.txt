cmake_minimum_required(VERSION 3.5)

project(ScreenTrackAndAction VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)

find_package(X11 REQUIRED)

find_package(frugally-deep CONFIG REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

find_library(xdo_LIBRARY NAMES xdo libxdo PATHS /usr/lib)
if (xdo_LIBRARY)
else(xdo_LIBRARY)
  message(FATAL_ERROR "Can't find xdo")
endif(xdo_LIBRARY)

pkg_search_module(TESSERACT REQUIRED tesseract)
pkg_search_module(LEPTONICA REQUIRED lept)


set(PROJECT_SOURCES
        main.cpp
        regionselect.cpp
        regionselect.h
        bottomwidget.h
        bottomwidget.cpp
        getpixmap.cpp
        getpixmap.h
        textinsert.cpp
        textinsert.h
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        scribblearea.h
        scribblearea.cpp
        recognizegesture.h
        recognizegesture.cpp
        capturemouse.h
        capturemouse.cpp
)
add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x050F00)
include_directories(${OpenCV_INCLUDE_DIRS} ${TESSERACT_INCLUDE_DIRS} ${LEPTONICA_INCLUDE_DIRS} ${X11_INCLUDE_DIR})

link_directories( ${TESSERACT_LIBRARY_DIRS} )
link_directories( ${LEPTONICA_LIBRARY_DIRS} )


link_directories( ${TESSERACT_LIBRARY_DIRS} )
link_directories( ${LEPTONICA_LIBRARY_DIRS} )

link_libraries(${OpenCV_LIBS} ${xdo_LIBRARY} ${X11_LIBRARIES})


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ScreenTrackAndAction
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ScreenTrackAndAction APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ScreenTrackAndAction SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ScreenTrackAndAction
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(ScreenTrackAndAction PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${TESSERACT_LIBRARIES} ${LEPTONICA_LIBRARIES} frugally-deep::fdeep Eigen3::Eigen)

set_target_properties(ScreenTrackAndAction PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ScreenTrackAndAction)
endif()
