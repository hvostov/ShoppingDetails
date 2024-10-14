cmake_minimum_required(VERSION 3.2)

project(ShoppingDetails VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_PREFIX_PATH "D:\\Qt\\6.7.0\\mingw_64\\lib\\cmake\\Qt6\\")
#set (CMAKE_PREFIX_PATH "D:\\Qt\\6.0.7\\msvc2019_64\\lib\\cmake\\Qt6\\")
#set (CMAKE_PREFIX_PATH "D:/Qt/6.0.7/mingw_64/lib/cmake/Qt6")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
set (CMAKE_PREFIX_PATH "D:\\Qt\\6.7.0\\mingw_64\\lib\\cmake\\")
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS AxContainer)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS AxContainer)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ShoppingDetails
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        filltable.h filltable.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ShoppingDetails APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ShoppingDetails SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ShoppingDetails
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(ShoppingDetails PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(ShoppingDetails PRIVATE Qt${QT_VERSION_MAJOR}::AxContainer)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ShoppingDetails)
endif()
set_target_properties(ShoppingDetails PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ShoppingDetails
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ShoppingDetails)
endif()
