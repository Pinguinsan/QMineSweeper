#**********************************************************************#
#    QMineSweeper.pro:                                                 #
#    Project configuration file                                        #
#    Copyright (c) 2016 Tyler Lewis                                    #
#**********************************************************************#
#    This is a configuration file for QMineSweeper:                    #
#    https://github.com/Pinguinsan/QMineSweeper                        #
#    This file houses the overall project configuration                #
#    The source code is released under the LGPL                        #
#                                                                      #
#    You should have received a copy of the GNU Lesser General         #
#    Public license along with QMineSweeper                            #
#    If not, see <http://www.gnu.org/licenses/>                        #
#**********************************************************************#

#-------------------------------------------------
#
# Project created by QtCreator 2016-01-21T14:46:40
#
#-------------------------------------------------

QT       += core gui widgets multimedia

CONFIG += c++14

INCLUDEPATH += include/

!android {
    LIBS += -ltjlutils
}

TARGET   = QMineSweeper
TEMPLATE = app

HEADERS += include/mainwindow.h \
           include/gamecontroller.h \
           include/minecoordinates.h \
           include/qminesweepericons.h \
           include/minecoordinatehash.h \
           include/eventtimer.h \
           include/qminesweeperutilities.h \
           include/qminesweepersoundeffects.h \
           include/qminesweeperstrings.h \
           include/boardresizewindow.h \
           include/qminesweeperbutton.h

SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/eventtimer.cpp \
           src/gamecontroller.cpp \
           src/minecoordinates.cpp \
           src/qminesweepericons.cpp \
           src/qminesweeperutilities.cpp \
           src/qminesweepersoundeffects.cpp \
           src/qminesweeperstrings.cpp \
           src/qminesweeperbutton.cpp

RESOURCES += resources/icons.qrc \
             resources/sounds.qrc

FORMS += forms/mainwindow.ui \
         forms/boardresizewindow.ui

win32 {
    RC_FILE += resources/qminesweeper.rc
    #DLL_LIB_NAME = tjlutils.dll
    #BIN_DIR = C:/Cygwin64/opt/QtBuilds/QMineSweeper/Debug/debug/
    #LIB_DIR = C:/Cygwin64/opt/LibraryBuilds/tjlutils/Debug/debug/
    #WINDEPLOY_QT = C:/Qt/5.7/mingw53_32/bin/windeployqt.exe
    #QMAKE_POST_LINK += $$quote(cmd /c $$quote(copy /y $${LIB_DIR}$${DLL_LIB_NAME} $${BIN_DIR}$$escape_expand(\n\t))) \
    #                   $$quote(cmd /c $$quote(cd $${BIN_DIR} && $${WINDEPLOY_QT}))
}

android {
    DISTFILES += Android/AndroidManifest.xml \
                 Android/gradle/wrapper/gradle-wrapper.jar \
                 Android/gradlew \
                 Android/res/values/libs.xml \
                 Android/build.gradle \
                 Android/gradle/wrapper/gradle-wrapper.properties \
                 Android/gradlew.bat

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/Android
}
