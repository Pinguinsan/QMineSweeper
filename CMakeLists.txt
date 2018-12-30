cmake_minimum_required (VERSION 3.1)

project(QMineSweeper CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if (POLICY CMP0071)
    cmake_policy(SET CMP0071 NEW)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (WIN32)
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set (DEPLOY_BUILD_ARGUMENT "--debug")
    else()
        set (DEPLOY_BUILD_ARGUMENT "--release")
    endif()

    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		set(WIN_COMPILER "MSVC")
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set(WIN_COMPILER "MINGW")
    endif()
    if (NOT WIN_COMPILER)
        message(FATAL_ERROR "Please specify -DWIN_COMPILER=MSVC or -DWIN_COMPILER=MINGW")
    endif()
    IF (WIN_COMPILER STREQUAL "MINGW")
        set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
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
        set(QT_ROOT "C:/Qt/5.12.0")
    endif()
    #Change this variable to the mingw version used by your Qt build version (NOT THE COMPILER PATH)
    if (WIN_COMPILER STREQUAL "MSVC")
        if (NOT QT_MSVC_VERSION)
            set (QT_MSVC_VERSION "msvc2017_64")
        endif()
    else()
        if (NOT QT_MINGW_VERSION)
            set(QT_MINGW_VERSION "mingw73_64")
        endif()
    endif()
else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()


set(QT_PACKAGES Qt5Widgets Qt5Gui Qt5Core Qt5Multimedia)
set(QT_LINK_LIBRARIES)
set(QT_LIBRARY_LIST)

add_definitions("-DUSE_QT_LOG")

if (WIN32 OR WIN64)
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


set (SOURCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src")
set (RESOURCES_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set (FORMS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/forms")
set (THIRD_PARTY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/third-party")

if (WIN32 OR WIN64)
    if (WIN_COMPILER STREQUAL "MSVC")
        add_subdirectory(${THIRD_PARTY_ROOT}/win32/getopt)
        set(PLATFORM_SPECIFIC_LIBS getopt-win32)
    else()
        set(PLATFORM_SPECIFIC_LIBS pthread)
    endif()
else()
    set (PLATFORM_SPECIFIC_LIBS pthread)
endif()

set (${PROJECT_NAME}_SOURCE_FILES
        "${SOURCE_ROOT}/Main.cpp"
        "${SOURCE_ROOT}/MainWindow.cpp"
        "${SOURCE_ROOT}/GameController.cpp"
        "${SOURCE_ROOT}/MineCoordinates.cpp"
        "${SOURCE_ROOT}/QmsIcons.cpp"
        "${SOURCE_ROOT}/QmsUtilities.cpp"
        "${SOURCE_ROOT}/QmsSoundEffects.cpp"
        "${SOURCE_ROOT}/QmsButton.cpp"
        "${SOURCE_ROOT}/QmsSettingsLoader.cpp"
        "${SOURCE_ROOT}/QmsApplicationSettings.cpp"
        "${SOURCE_ROOT}/BoardResizeWidget.cpp"
        "${SOURCE_ROOT}/MouseMoveableQMainWindow.cpp"
        "${SOURCE_ROOT}/MouseMoveableQWidget.cpp"
		"${SOURCE_ROOT}/AboutApplicationWidget.cpp"
        "${SOURCE_ROOT}/QmsGameState.cpp"
        "${SOURCE_ROOT}/AutoUpdateLCD.cpp"
     "${SOURCE_ROOT}/StaticLogger.cpp"
     "${SOURCE_ROOT}/PrettyPrinter.cpp")

set(${PROJECT_NAME}_HEADER_FILES
        "${SOURCE_ROOT}/QmsGameState.hpp"
        "${SOURCE_ROOT}/MouseMoveableQWidget.hpp"
        "${SOURCE_ROOT}/MouseMoveableQMainWindow.hpp"
        "${SOURCE_ROOT}/BoardResizeWidget.hpp"
        "${SOURCE_ROOT}/QmsApplicationSettings.hpp"
        "${SOURCE_ROOT}/QmsSettingsLoader.hpp"
        "${SOURCE_ROOT}/QmsButton.hpp"
        "${SOURCE_ROOT}/QmsSoundEffects.hpp"
        "${SOURCE_ROOT}/QmsUtilities.hpp"
        "${SOURCE_ROOT}/QmsIcons.hpp"
        "${SOURCE_ROOT}/QmsStrings.hpp"
        "${SOURCE_ROOT}/ProgramOption.hpp"
        "${SOURCE_ROOT}/MineCoordinates.hpp"
        "${SOURCE_ROOT}/GameController.hpp"
        "${SOURCE_ROOT}/MainWindow.hpp"
        "${SOURCE_ROOT}/AboutApplicationWidget.hpp"
        "${SOURCE_ROOT}/Version.hpp"
        "${SOURCE_ROOT}/EventTimer.hpp"
        "${SOURCE_ROOT}/GlobalDefinitions.hpp"
        "${SOURCE_ROOT}/MineCoordinateHash.hpp"
        "${SOURCE_ROOT}/ChangeAwareValue.hpp"
        "${SOURCE_ROOT}/AutoUpdateLCD.hpp"
        "${SOURCE_ROOT}/AbstractEventNotifier.hpp"
        "${SOURCE_ROOT}/StaticLogger.hpp"
        "${SOURCE_ROOT}/PrettyPrinter.hpp")


set (${PROJECT_NAME}_FORMS
        "${FORMS_ROOT}/MainWindow.ui"
        "${FORMS_ROOT}/BoardResizeWidget.ui"
		"${FORMS_ROOT}/AboutApplicationWidget.ui")

set (${PROJECT_NAME}_RESOURCES
        "${RESOURCES_ROOT}/icons.qrc"
        "${RESOURCES_ROOT}/sounds.qrc"
        "${RESOURCES_ROOT}/translations.qrc")


qt5_wrap_ui (${PROJECT_NAME}_FORMS_MOC  ${${PROJECT_NAME}_FORMS})
qt5_add_resources(${PROJECT_NAME}_RESOURCES_RCC ${${PROJECT_NAME}_RESOURCES})

if (WIN32)
    add_executable(${PROJECT_NAME}
            WIN32
            ${${PROJECT_NAME}_SOURCE_FILES}
            ${${PROJECT_NAME}_HEADER_FILES}
            ${${PROJECT_NAME}_FORMS}
            ${${PROJECT_NAME}_RESOURCES_RCC}
            "${RESOURCES_ROOT}/QMineSweeper.rc")


    if (WIN_COMPILER STREQUAL "MSVC")
        add_custom_command(TARGET ${PROJECT_NAME}
                POST_BUILD
                COMMAND "${QT_ROOT}/${QT_MSVC_VERSION}/bin/windeployqt.exe" "${CMAKE_BINARY_DIR}/" ${DEPLOY_BUILD_ARGUMENT})
    elseif(WIN_COMPILER STREQUAL "MINGW")
        add_custom_command(TARGET ${PROJECT_NAME}
                POST_BUILD
                COMMAND "${QT_ROOT}/${QT_MINGW_VERSION}/bin/windeployqt.exe" "${CMAKE_BINARY_DIR}/" ${DEPLOY_BUILD_ARGUMENT})

    endif()

else()

    add_executable(${PROJECT_NAME}
            ${${PROJECT_NAME}_SOURCE_FILES}
            ${${PROJECT_NAME}_HEADER_FILES}
            ${${PROJECT_NAME}_FORMS}
            ${${PROJECT_NAME}_RESOURCES_RCC})
endif()


target_link_libraries (${PROJECT_NAME}
        ${QT_LINK_LIBRARIES}
        ${PLATFORM_SPECIFIC_LIBS})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

set(DESKTOP_FILE_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/utility/${PROJECT_NAME}.desktop")
set(CONFIGURATION_FILE_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/utility/${PROJECT_NAME}.conf")
set(ICON_FILE_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/resources/${PROJECT_NAME}.png")

if(${CMAKE_SYSTEM_NAME} MATCHES Linux|.*BSD|DragonFly)
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
    install(DIRECTORY DESTINATION "/etc/${PROJECT_NAME}/")
    install(FILES "${CONFIGURATION_FILE_LOCATION}" DESTINATION "/etc/${PROJECT_NAME}")
    install(DIRECTORY DESTINATION "$ENV{HOME}/Desktop")
    install(FILES "${DESKTOP_FILE_LOCATION}" DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications")
    install(FILES "${DESKTOP_FILE_LOCATION}" DESTINATION "$ENV{HOME}/Desktop")
    install(FILES "${ICON_FILE_LOCATION}" DESTINATION "${CMAKE_INSTALL_PREFIX}/share/pixmaps")
endif()

