#CMakeLists.txt -> QMineSweeper

cmake_minimum_required (VERSION 3.2)
set (PROJECT_NAME QMineSweeper)
project(${PROJECT_NAME} CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if (CYGWIN)
    set(Qt5Core_DIR "/cygdrive/c/Qt/5.9/mingw53_32/lib/cmake/Qt5Core")
    set(Qt5Widgets_DIR "/cygdrive/c/Qt/5.9/mingw53_32/lib/cmake/Qt5Widgets")
    set(Qt5Gui_DIR "/cygdrive/c/Qt/5.9/mingw53_32/lib/cmake/Qt5Gui")
    set(Qt5Multimedia_DIR "/cygdrive/c/Qt/5.9/mingw53_32/lib/cmake/Qt5Multimedia")
    set(Qt5SerialPort_DIR "/cygdrive/c/Qt/5.9/mingw53_32/lib/cmake/Qt5SerialPort")
    set(Qt5_DIR "/cygdrive/c/Qt/5.9/mingw53_32/lib/cmake/Qt5")
    set(QT_QMAKE_EXECUTABLE "/cygdrive/c/Qt/5.9/mingw53_32/bin/qmake.exe")
    #set (CMAKE_MODULE_PATH "/c/cygdrive/Qt/5.9/mingw53_32/bin")
elseif(WIN32)
    set(Qt5Core_DIR "C:/Qt/5.9/mingw53_32/lib/cmake/Qt5Core")
    set(Qt5Widgets_DIR "C:/Qt/5.9/mingw53_32/lib/cmake/Qt5Widgets")
    set(Qt5Gui_DIR "C:/Qt/5.9/mingw53_32/lib/cmake/Qt5Gui")
    set(Qt5Multimedia_DIR "C:/Qt/5.9/mingw53_32/lib/cmake/Qt5Multimedia")
    set(Qt5SerialPort_DIR "C:/Qt/5.9/mingw53_32/lib/cmake/Qt5SerialPort")
    set(Qt5_DIR "C:/Qt/5.9/mingw53_32/lib/cmake/Qt5")
    set(QT_QMAKE_EXECUTABLE "C:/Qt/5.9/mingw53_32/bin/qmake.exe")
    #set (CMAKE_MODULE_PATH "C:/Qt/5.9/mingw53_32/bin")

endif()

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Multimedia REQUIRED)

set (${PROJECT_NAME}_SOURCES
        src/Main.cpp
        src/MainWindow.cpp src/MainWindow.h
        src/GameController.cpp src/GameController.h
        src/MineCoordinates.cpp src/MineCoordinates.h
        src/QmsIcons.cpp src/QmsIcons.h
        src/QmsUtilities.cpp src/QmsUtilities.h
        src/QmsSoundEffects.cpp src/QmsSoundEffects.h
        src/QmsStrings.cpp src/QmsStrings.h
        src/QmsButton.cpp src/QmsButton.h
        src/QmsSettingsLoader.cpp src/QmsSettingsLoader.h
        src/QmsApplicationSettings.cpp src/QmsApplicationSettings.h
        src/BoardResizeWidget.cpp src/BoardResizeWidget.h
        src/AboutQmsWidget.cpp src/AboutQmsWidget.h
        src/MouseMoveableQMainWindow.cpp src/MouseMoveableQMainWindow.h
        src/MouseMoveableQWidget.cpp src/MouseMoveableQWidget.h
        src/QmsGameState.cpp src/QmsGameState.h
        src/Version.h
        src/EventTimer.h
        src/GlobalDefinitions.h
        src/MineCoordinateHash.h
        )



set (${PROJECT_NAME}_FORMS
        forms/MainWindow.ui
        forms/BoardResizeWidget.ui
        forms/AboutQmsWidget.ui
        )

set (${PROJECT_NAME}_RESOURCES
        resources/icons.qrc
        resources/sounds.qrc
        resources/translations.qrc
        )

qt5_wrap_ui (QMINESWEEPER_FORMS_MOC  ${${PROJECT_NAME}_FORMS})
qt5_add_resources(${PROJECT_NAME}_RESOURCES_RCC ${${PROJECT_NAME}_RESOURCES})

if (WIN32 OR CMAKE)
add_executable(QMineSweeper
        ${${PROJECT_NAME}_SOURCES}
        ${${PROJECT_NAME}_HEADERS}
        ${${PROJECT_NAME}_FORMS}
        ${${PROJECT_NAME}_RESOURCES_RCC}
        resources/QMineSweeper.rc
        )

target_link_libraries (${PROJECT_NAME}
        Qt5::Widgets
        Qt5::Gui
        Qt5::Core
        Qt5::Multimedia
        )
else()
add_executable(QMineSweeper
        ${${PROJECT_NAME}_SOURCES}
        ${${PROJECT_NAME}_HEADERS}
        ${${PROJECT_NAME}_FORMS}
        ${${PROJECT_NAME}_RESOURCES_RCC}
        )

target_link_libraries (${PROJECT_NAME}
        Qt5::Widgets
        Qt5::Gui
        Qt5::Core
        Qt5::Multimedia
        pthread
        )
endif()

