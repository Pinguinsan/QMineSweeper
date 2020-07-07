SET(QT_MISSING True)
# msvc only; mingw will need different logic
IF(MSVC OR MINGW)
    # look for user-registry pointing to qtcreator
    GET_FILENAME_COMPONENT(QT_BIN [HKEY_CURRENT_USER\\Software\\Classes\\Applications\\QtProject.QtCreator.cpp\\shell\\Open\\Command] PATH)

    # get root path so we can search for 5.3, 5.4, 5.5, etc
    STRING(REPLACE "/Tools" ";" QT_BIN "${QT_BIN}")
    LIST(GET QT_BIN 0 QT_BIN)
    FILE(GLOB QT_VERSIONS "${QT_BIN}/5.*")
    LIST(SORT QT_VERSIONS)

    # assume the latest version will be last alphabetically
    LIST(REVERSE QT_VERSIONS)

    LIST(GET QT_VERSIONS 0 QT_VERSION)

    # fix any double slashes which seem to be common
    STRING(REPLACE "//" "/"  QT_VERSION "${QT_VERSION}")

    # do some math trickery to guess folder
    # - qt uses (e.g.) "msvc2012"
    # - cmake uses (e.g.) "1800"
    # - see also https://cmake.org/cmake/help/v3.0/variable/MSVC_VERSION.html
    IF (MSVC)
        MATH(EXPR QT_COMPILER_ID "2000 + (${MSVC_VERSION} - 600) / 100")
    ELSEIF(MINGW)
        #Convoluted check to find mingw compiler under Qt folder
        string(REPLACE "." "" QT_COMPILER_ID ${CMAKE_CXX_COMPILER_VERSION})
        #Strip off trailing 0 to check if version 7.3.0 is 73 instead of 730
        IF(QT_COMPILER_ID MATCHES ".*0")
            string(LENGTH ${QT_COMPILER_ID} QT_COMPILER_ID_LENGTH)
            math(EXPR QT_COMPILER_ID_NEW_LENGTH "${QT_COMPILER_ID_LENGTH}-1")
            string(SUBSTRING ${QT_COMPILER_ID} 0 ${QT_COMPILER_ID_NEW_LENGTH} TEMP_CHECK_1)
        endif()
        IF(CMAKE_SYSTEM_PROCESSOR MATCHES 64)
            SET(TEMP_CHECK "mingw${QT_COMPILER_ID}_64")
            IF (TEMP_CHECK_1)
                SET(TEMP_CHECK_1 "mingw${TEMP_CHECK_1}_64")
            ENDIF()
        ELSE()
            SET(TEMP_CHECK "mingw${QT_COMPILER_ID}_32")
            IF (TEMP_CHECK_1)
                SET(TEMP_CHECK_1 "mingw${TEMP_CHECK_1}_32")
            ENDIF()
        ENDIF()

        set(FULL_TEMP_CHECK "${QT_VERSION}/${TEMP_CHECK}")
        IF (EXISTS "${FULL_TEMP_CHECK}")
            message(STATUS "Found compiler at ${FULL_TEMP_CHECK}")
            set(QT_COMPILER_DIR ${TEMP_CHECK})
        ELSE()
            IF (TEMP_CHECK_1)
                set(FULL_TEMP_CHECK_1 "${QT_VERSION}/${TEMP_CHECK_1}")
                IF (EXISTS "${FULL_TEMP_CHECK_1}")
                    message(STATUS "Found compiler at ${FULL_TEMP_CHECK_1}")
                    set(QT_COMPILER_DIR ${TEMP_CHECK_1})
                ENDIF()
            ENDIF()
        ENDIF()

        #set(QT_COMPILER_ID ${CMAKE_CXX_COMPILER_VERSION})
    ENDIF()
    # check for 64-bit os
    # may need to be removed for older compilers as it wasn't always offered
    IF(MSVC)
        IF(CMAKE_SYSTEM_PROCESSOR MATCHES 64)
            SET(QT_COMPILER_ID "${QT_COMPILER_ID}_64")
        ENDIF()
        SET(QT_COMPILER_DIR "msvc${QT_COMPILER_ID}")
        SET(QT_ROOT_PATH "${QT_VERSION}")
        SET(QT_FULL_PATH "${QT_ROOT_PATH}/${QT_COMPILER_DIR}")
        SET(QT_MISSING False)
    ELSEIF(MINGW)
        SET(QT_ROOT_PATH "${QT_VERSION}")
        SET(QT_FULL_PATH "${QT_ROOT_PATH}/${QT_COMPILER_DIR}")
        SET(QT_MISSING False)
    ENDIF()
ENDIF()

# use Qt_DIR approach so you can find Qt after cmake has been invoked
IF(NOT QT_MISSING)
    MESSAGE("-- Qt FOUND: ${QT_FULL_PATH}")
    SET(Qt5_DIR "${QT_PATH}/lib/cmake/Qt5/")
ENDIF()
