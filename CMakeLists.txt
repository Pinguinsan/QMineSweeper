#CMakeLists.txt -> QMineSweeper

cmake_minimum_required (VERSION 3.2)
set (PROJECT_NAME QMineSweeper)
project(${PROJECT_NAME} CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if (POLICY CMP0071)
    cmake_policy(SET CMP0071 NEW)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (WIN32)

    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		set(WIN_COMPILER "MSVC")
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set(WIN_COMPILER "MINGW")
    endif()
    if (NOT WIN_COMPILER)
        message(FATAL_ERROR "Please specify -DWIN_COMPILER=MSVC or -DWIN_COMPILER=MINGW")
    endif()
    IF (WIN_COMPILER STREQUAL "MINGW")
        set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
        set(CMAKE_CXX_FLAGS_DEBUG "-g")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    endif()
        if (WIN_COMPILER STREQUAL "MSVC")
        set (WINDOWS_COMPILER MSVC)
        set(COVERAGE_LINK_FLAGS  "/SUBSYSTEM:WINDOWS ")
        set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")
    else()
        set (WINDOWS_COMPILER MINGW)
        set(COVERAGE_LINK_FLAGS  "-mwindows")
        set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")
    endif()


    #Change this variable to the current Qt root!
    if (NOT QT_ROOT)
        set(QT_ROOT "C:/Qt/5.10.0")
    endif()
    #Change this variable to the mingw version used by your Qt build version (NOT THE COMPILER PATH)
    if (WIN_COMPILER STREQUAL "MSVC")
        if (NOT QT_MSVC_VERSION)
            set (QT_MSVC_VERSION "msvc2017_64")
        endif()
    else()
        if (NOT QT_MINGW_VERSION)
            set(QT_MINGW_VERSION "mingw53_32")
        endif()
    endif()
else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()


set(QT_PACKAGES Qt5Widgets Qt5Gui Qt5Core Qt5Multimedia Qt5SerialPort)
set(QT_LINK_LIBRARIES)
set(QT_LIBRARY_LIST)

if (WIN32)
        if (WIN_COMPILER STREQUAL "MSVC")
            set(Qt5_DIR "${QT_ROOT}/${QT_MSVC_VERSION}/lib/cmake/Qt5")
            set(QT_QMAKE_EXECUTABLE "${QT_ROOT}/${QT_MSVC_VERSION}/bin/qmake.exe")
            message(STATUS "QT_ROOT = ${QT_ROOT}")
        else()
            set(Qt5_DIR "${QT_ROOT}/${QT_MINGW_VERSION}/lib/cmake/Qt5")
            set(QT_QMAKE_EXECUTABLE "${QT_ROOT}/${QT_MINGW_VERSION}/bin/qmake.exe")
            message(STATUS "QT_ROOT = ${QT_ROOT}")
        endif()
endif()

foreach(Qt5Package ${QT_PACKAGES})
    set(Qt5PackageBase "")
    string(SUBSTRING ${Qt5Package} 0 3 Qt5PackageBase)
    string(SUBSTRING ${Qt5Package} 3 -1 Qt5PackageTail)
    string(CONCAT Qt5PackageLib "${Qt5PackageBase}" "::" "${Qt5PackageTail}")
    list(APPEND QT_LIBRARY_LIST "${Qt5PackageTail}")
    list(APPEND QT_LINK_LIBRARIES "${Qt5PackageLib}")
    if (WIN32)
		if (WIN_COMPILER STREQUAL "MSVC")
            set(${Qt5Package}_DIR "${QT_ROOT}/${QT_MSVC_VERSION}/lib/cmake/${Qt5Package}")
		else()
            set(${Qt5Package}_DIR "${QT_ROOT}/${QT_MINGW_VERSION}/lib/cmake/${Qt5Package}")
        endif()
        message(STATUS "${Qt5Package}_DIR = ${${Qt5Package}_DIR}")
    endif()
    find_package(${Qt5Package})
    if (${Qt5Package}_FOUND)
        message(STATUS "${Qt5Package}_INCLUDE_DIRS: ${${Qt5Package}_INCLUDE_DIRS}}")
        message(STATUS "${Qt5Package}_LIBRARIES: ${${Qt5Package}_LIBRARIES}")
        message(STATUS "${Qt5Package}_VERSION: ${${Qt5Package}_VERSION}")
        include_directories(${${Qt5Package}_INCLUDE_DIRS})
    endif()
endforeach(Qt5Package)


set (SOURCE_ROOT src)
set (RESOURCES_ROOT resources)
set (FORMS_ROOT forms)

set (${PROJECT_NAME}_SOURCE_FILES
        ${SOURCE_ROOT}/Main.cpp
        ${SOURCE_ROOT}/MainWindow.cpp
        ${SOURCE_ROOT}/GameController.cpp
        ${SOURCE_ROOT}/MineCoordinates.cpp
        ${SOURCE_ROOT}/QmsIcons.cpp
        ${SOURCE_ROOT}/QmsUtilities.cpp
        ${SOURCE_ROOT}/QmsSoundEffects.cpp
        ${SOURCE_ROOT}/QmsButton.cpp
        ${SOURCE_ROOT}/QmsSettingsLoader.cpp
        ${SOURCE_ROOT}/QmsApplicationSettings.cpp
        ${SOURCE_ROOT}/BoardResizeWidget.cpp
        ${SOURCE_ROOT}/MouseMoveableQMainWindow.cpp
        ${SOURCE_ROOT}/MouseMoveableQWidget.cpp
		${SOURCE_ROOT}/AboutApplicationWidget.cpp
        ${SOURCE_ROOT}/QmsGameState.cpp
        ${SOURCE_ROOT}/AutoUpdateLCD.cpp)

set(${PROJECT_NAME}_HEADER_FILES
        ${SOURCE_ROOT}/QmsGameState.h
         ${SOURCE_ROOT}/MouseMoveableQWidget.h
         ${SOURCE_ROOT}/MouseMoveableQMainWindow.h
         ${SOURCE_ROOT}/BoardResizeWidget.h
         ${SOURCE_ROOT}/QmsApplicationSettings.h
         ${SOURCE_ROOT}/QmsSettingsLoader.h
         ${SOURCE_ROOT}/QmsButton.h
         ${SOURCE_ROOT}/QmsSoundEffects.h
         ${SOURCE_ROOT}/QmsUtilities.h
         ${SOURCE_ROOT}/QmsIcons.h
        ${SOURCE_ROOT}/QmsStrings.h
         ${SOURCE_ROOT}/MineCoordinates.h
         ${SOURCE_ROOT}/GameController.h
         ${SOURCE_ROOT}/MainWindow.h
		 ${SOURCE_ROOT}/AboutApplicationWidget.h
        ${SOURCE_ROOT}/Version.h
        ${SOURCE_ROOT}/EventTimer.h
        ${SOURCE_ROOT}/GlobalDefinitions.h
        ${SOURCE_ROOT}/MineCoordinateHash.h
        ${SOURCE_ROOT}/ChangeAwareValue.h
        ${SOURCE_ROOT}/AutoUpdateLCD.h)


set (${PROJECT_NAME}_FORMS
        ${FORMS_ROOT}/MainWindow.ui
        ${FORMS_ROOT}/BoardResizeWidget.ui
		${FORMS_ROOT}/AboutApplicationWidget.ui)

set (${PROJECT_NAME}_RESOURCES
        ${RESOURCES_ROOT}/icons.qrc
        ${RESOURCES_ROOT}/sounds.qrc
        ${RESOURCES_ROOT}/translations.qrc)


qt5_wrap_ui (${PROJECT_NAME}_FORMS_MOC  ${${PROJECT_NAME}_FORMS})
qt5_add_resources(${PROJECT_NAME}_RESOURCES_RCC ${${PROJECT_NAME}_RESOURCES})

if (WIN32)
add_executable(${PROJECT_NAME}
        WIN32
        ${${PROJECT_NAME}_SOURCE_FILES}
        ${${PROJECT_NAME}_HEADER_FILES}
        ${${PROJECT_NAME}_FORMS}
        ${${PROJECT_NAME}_RESOURCES_RCC}
        resources/QMineSweeper.rc)


if (WIN_COMPILER STREQUAL "MSVC")
    add_custom_command(TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND "${QT_ROOT}/${QT_MSVC_VERSION}/bin/windeployqt.exe" ARGS "${CMAKE_BINARY_DIR}/")
else()
    add_custom_command(TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND "${QT_ROOT}/${QT_MINGW_VERSION}/bin/windeployqt.exe" ARGS "${CMAKE_BINARY_DIR}/")

endif()

else()

add_executable(QMineSweeper
        ${${PROJECT_NAME}_SOURCE_FILES}
        ${${PROJECT_NAME}_HEADER_FILES}
        ${${PROJECT_NAME}_FORMS}
        ${${PROJECT_NAME}_RESOURCES_RCC})

target_link_libraries (${PROJECT_NAME}
        pthread)
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

qt5_use_modules(${PROJECT_NAME} ${QT_LIBRARY_LIST})
