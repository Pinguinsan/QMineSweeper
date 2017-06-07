#CMakeLists.txt -> QMineSweeper

cmake_minimum_required (VERSION 3.2) 
project(QMineSweeper CXX)

set(CMAKE_BUILD_TYPE Debug)
set(SOURCE_ROOT ${PROJECT_SOURCE_DIR})
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
    set(GCC_ADDITIONAL_COMPILE_FLAGS "-fsanitize=undefined")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_ADDITIONAL_COMPILE_FLAGS}")
endif()

set (QMINESWEEPER_SOURCES "${SOURCE_ROOT}/src/main.cpp"
                          "${SOURCE_ROOT}/src/mainwindow.cpp"
                          "${SOURCE_ROOT}/src/gamecontroller.cpp"
                          "${SOURCE_ROOT}/src/minecoordinates.cpp"
                          "${SOURCE_ROOT}/src/qminesweepericons.cpp"
                          "${SOURCE_ROOT}/src/qminesweeperutilities.cpp"
                          "${SOURCE_ROOT}/src/qminesweepersoundeffects.cpp"
                          "${SOURCE_ROOT}/src/qminesweeperstrings.cpp"
                          "${SOURCE_ROOT}/src/qminesweeperbutton.cpp"
                          "${SOURCE_ROOT}/src/qminesweepersettingsloader.cpp"
                          "${SOURCE_ROOT}/src/qminesweeperapplicationsettings.cpp"
                          "${SOURCE_ROOT}/src/boardresizewindow.cpp")

    
set (QMINESWEEPER_HEADERS "${SOURCE_ROOT}/src/mainwindow.h"
                          "${SOURCE_ROOT}/src/gamecontroller.h"
                          "${SOURCE_ROOT}/src/minecoordinates.h"
                          "${SOURCE_ROOT}/src/qminesweepericons.h"
                          "${SOURCE_ROOT}/src/minecoordinatehash.h"
                          "${SOURCE_ROOT}/src/eventtimer.h"
                          "${SOURCE_ROOT}/src/qminesweeperutilities.h"
                          "${SOURCE_ROOT}/src/qminesweepersoundeffects.h"
                          "${SOURCE_ROOT}/src/qminesweeperstrings.h"
                          "${SOURCE_ROOT}/src/boardresizewindow.h"
                          "${SOURCE_ROOT}/src/qminesweeperbutton.h"
                          "${SOURCE_ROOT}/src/qminesweepersettingsloader.h"
                          "${SOURCE_ROOT}/src/qminesweeperapplicationsettings.h"
                          "${SOURCE_ROOT}/src/globaldefinitions.h")


if (WIN32)
    set (QMINESWEEPER_HEADERS ${QMINESWEEPER_HEADERS} ${SOURCE_ROOT}/src/version.h)
endif()


set (QMINESWEEPER_FORMS "${SOURCE_ROOT}/forms/mainwindow.ui"
                        "${SOURCE_ROOT}/forms/boardresizewindow.ui")

set (QMINESWEEPER_RESOURCES "${SOURCE_ROOT}/resources/icons.qrc"
                            "${SOURCE_ROOT}/resources/sounds.qrc"
                            "${SOURCE_ROOT}/resources/translations.qrc")

qt5_wrap_cpp(QMINESWEEPER_HEADERS_MOC "${QMINESWEEPER_HEADERS}")
qt5_wrap_ui (QMINESWEEPER_FORMS_MOC  "${QMINESWEEPER_FORMS}")
qt5_add_resources(QMINESWEEPER_RESOURCES_RCC "${QMINESWEEPER_RESOURCES}")

add_executable(QMineSweeper "${QMINESWEEPER_SOURCES}"
                            "${QMINESWEEPER_HEADERS}"
                            "${QMINESWEEPER_FORMS}"
                            "${QMINESWEEPER_RESOURCES_RCC}")
target_link_libraries (QMineSweeper Qt5::Widgets Qt5::Gui Qt5::Core Qt5::Multimedia pthread)
