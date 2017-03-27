#CMakeLists.txt -> QMineSweeper

cmake_minimum_required (VERSION 3.2) 
project(QMineSweeper CXX)

set(CMAKE_BUILD_TYPE Release)
set(SOURCE_BASE ${PROJECT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Multimedia REQUIRED)

if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
    set (X86 TRUE)
else()
    set (X86 FALSE)
endif()

if (CYGWIN OR WIN32 OR (NOT X86))
    set(GCC_ADDITIONAL_COMPILE_FLAGS "")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_ADDITIONAL_COMPILE_FLAGS}")
else()
    set(GCC_ADDITIONAL_COMPILE_FLAGS "-fsanitize=undefined -frecord-gcc-switches")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_ADDITIONAL_COMPILE_FLAGS}")
endif()

include_directories("${SOURCE_BASE}/include")


set (QMINESWEEPER_SOURCES "${SOURCE_BASE}/src/main.cpp"
                            "${SOURCE_BASE}/src/mainwindow.cpp"
                            "${SOURCE_BASE}/src/gamecontroller.cpp"
                            "${SOURCE_BASE}/src/minecoordinates.cpp"
                            "${SOURCE_BASE}/src/qminesweepericons.cpp"
                            "${SOURCE_BASE}/src/qminesweeperutilities.cpp"
                            "${SOURCE_BASE}/src/qminesweepersoundeffects.cpp"
                            "${SOURCE_BASE}/src/qminesweeperstrings.cpp"
                            "${SOURCE_BASE}/src/qminesweeperbutton.cpp")

    
set (QMINESWEEPER_HEADERS "${SOURCE_BASE}/include/boardresizewindow.h"
                           "${SOURCE_BASE}/include/minecoordinates.h"
                            "${SOURCE_BASE}/include/qminesweeperbutton.h"
                            "${SOURCE_BASE}/include/eventtimer.h"          
                            "${SOURCE_BASE}/include/qminesweepericons.h"
                            "${SOURCE_BASE}/include/gamecontroller.h"      
                            "${SOURCE_BASE}/include/mainwindow.h"          
                            "${SOURCE_BASE}/include/qminesweeperstrings.h"
                            "${SOURCE_BASE}/include/minecoordinatehash.h" 
                            "${SOURCE_BASE}/include/qminesweeperutilities.h"
                            "${SOURCE_BASE}/include/qminesweepersoundeffects.h")


set (QMINESWEEPER_FORMS "${SOURCE_BASE}/forms/mainwindow.ui"
                        "${SOURCE_BASE}/forms/boardresizewindow.ui")

set (QMINESWEEPER_RESOURCES "${SOURCE_BASE}/resources/icons.qrc"
                            "${SOURCE_BASE}/resources/sounds.qrc")

qt5_wrap_cpp(QMINESWEEPER_HEADERS_MOC "${QMINESWEEPER_HEADERS}")
qt5_wrap_ui (QMINESWEEPER_FORMS_MOC  "${QMINESWEEPER_FORMS}")
qt5_add_resources(QMINESWEEPER_RESOURCES_RCC "${QMINESWEEPER_RESOURCES}")

set (QMINESWEEPER_SOURCES_LIB "${QMINESWEEPER_SOURCES}")
set (QMINESWEEPER_HEADER_LIB "${QMINESWEEPER_HEADERS}")
set (QMINESWEEPER_FORMS_LIB  "${QMINESWEEPER_FORMS}")

add_library (QMINESWEEPER_SHARED_LIB  "${QMINESWEEPER_SOURCES_LIB}"
                                      "${QMINESWEEPER_HEADER_LIB}"
                                      "${QMINESWEEPER_FORMS_LIB}")
target_link_libraries (QMINESWEEPER_SHARED_LIB Qt5::Widgets Qt5::Gui Qt5::Core Qt5::Multimedia pthread)

add_executable(QMineSweeper "${QMINESWEEPER_SOURCES}"
                            "${QMINESWEEPER_HEADERS}"
                            "${QMINESWEEPER_RESOURCES_RCC}")
target_link_libraries (QMineSweeper QMINESWEEPER_SHARED_LIB pthread)
