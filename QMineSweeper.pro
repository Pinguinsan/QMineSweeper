#**********************************************************************#
#    QMineSweeper.pro:                                                 #
#    Project configuration file                                        #
#    Copyright (c) 2017 Tyler Lewis                                    #
#**********************************************************************#
#    This is a configuration file for QMineSweeper:                    #
#    https://github.com/tlewiscpp/QMineSweeper                         #
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

TARGET   = QMineSweeper
TEMPLATE = app

HEADERS += src/mainwindow.h \
           src/gamecontroller.h \
           src/minecoordinates.h \
           src/qminesweepericons.h \
           src/minecoordinatehash.h \
           src/eventtimer.h \
           src/qminesweeperutilities.h \
           src/qminesweepersoundeffects.h \
           src/qminesweeperstrings.h \
           src/boardresizewindow.h \
           src/qminesweeperbutton.h \
    src/qminesweepersettingsloader.h

SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/gamecontroller.cpp \
           src/minecoordinates.cpp \
           src/qminesweepericons.cpp \
           src/qminesweeperutilities.cpp \
           src/qminesweepersoundeffects.cpp \
           src/qminesweeperstrings.cpp \
           src/qminesweeperbutton.cpp \
    src/qminesweepersettingsloader.cpp

RESOURCES += resources/icons.qrc \
             resources/sounds.qrc

FORMS += forms/mainwindow.ui \
         forms/boardresizewindow.ui

win32 {
    RC_FILE += resources/qminesweeper.rc
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
