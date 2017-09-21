#CMakeLists.txt -> QMineSweeper

cmake_minimum_required (VERSION 3.2)
set (PROJECT_NAME QMineSweeper)
project(${PROJECT_NAME} CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


if (WIN32 OR CYGWIN)
    set(Qt5Core_DIR "C:/Qt/5.9/mingw53_32/lib/cmake/Qt5Core")
    set(Qt5Widgets_DIR "C:/Qt/5.9/mingw53_32/lib/cmake/Qt5Widgets")
    set(Qt5Gui_DIR "C:/Qt/5.9/mingw53_32/lib/cmake/Qt5Gui")
    set(Qt5Multimedia_DIR "C:/Qt/5.9/mingw53_32/lib/cmake/Qt5Multimedia")
    set(Qt5SerialPort_DIR "C:/Qt/5.9/mingw53_32/lib/cmake/Qt5SerialPort")
    set(Qt5_DIR "C:/Qt/5.9/mingw53_32/lib/cmake/Qt5")
    set(QT_QMAKE_EXECUTABLE "C:/Qt/5.9/mingw53_32/bin/qmake.exe")
    #set (CMAKE_MODULE_PATH "C:\Qt\5.9\mingw53_32\bin")
endif()

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Multimedia REQUIRED)

set (${PROJECT_NAME}_SOURCES
        src/main.cpp
      src/mainwindow.cpp src/mainwindow.h
      src/gamecontroller.cpp src/gamecontroller.h
      src/minecoordinates.cpp src/minecoordinates.h
      src/qmsicons.cpp src/qmsicons.h
      src/qmsutilities.cpp src/qmsutilities.h
      src/qmssoundeffects.cpp src/qmssoundeffects.h
      src/qmsstrings.cpp src/qmsstrings.h
      src/qmsbutton.cpp src/qmsbutton.h
      src/qmssettingsloader.cpp src/qmssettingsloader.h
      src/qmsapplicationsettings.cpp src/qmsapplicationsettings.h
      src/boardresizewidget.cpp src/boardresizewidget.h
      src/aboutqmswidget.cpp src/aboutqmswidget.h
        src/mousemoveableqmainwindow.cpp src/mousemoveableqmainwindow.h
        src/mousemoveableqwidget.cpp src/mousemoveableqwidget.h
        src/qmsgamestate.cpp src/qmsgamestate.h
        src/version.h
        src/eventtimer.h
        src/globaldefinitions.h
        src/minecoordinatehash.h
        )



set (${PROJECT_NAME}_FORMS
        forms/mainwindow.ui
        forms/boardresizewidget.ui
        forms/aboutqmswidget.ui
        )

set (${PROJECT_NAME}_RESOURCES
        resources/icons.qrc
        resources/sounds.qrc
        resources/translations.qrc
        )

qt5_wrap_ui (QMINESWEEPER_FORMS_MOC  ${${PROJECT_NAME}_FORMS})
qt5_add_resources(${PROJECT_NAME}_RESOURCES_RCC ${${PROJECT_NAME}_RESOURCES})

add_executable(QMineSweeper
        ${${PROJECT_NAME}_SOURCES}
        ${${PROJECT_NAME}_HEADERS}
        ${${PROJECT_NAME}_FORMS}
        ${${PROJECT_NAME}_RESOURCES_RCC}
        )

if (WIN32)
target_link_libraries (${PROJECT_NAME}
        Qt5::Widgets
        Qt5::Gui
        Qt5::Core
        Qt5::Multimedia
        )
else()
target_link_libraries (${PROJECT_NAME}
        Qt5::Widgets
        Qt5::Gui
        Qt5::Core
        Qt5::Multimedia
        pthread
        )
endif()

