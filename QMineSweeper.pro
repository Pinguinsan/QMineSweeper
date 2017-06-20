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
           src/minecoordinatehash.h \
           src/eventtimer.h \
    src/globaldefinitions.h \
    src/qmsapplicationsettings.h \
    src/qmsbutton.h \
    src/qmsicons.h \
    src/qmssettingsloader.h \
    src/qmssoundeffects.h \
    src/qmsstrings.h \
    src/qmsutilities.h \
    src/qmsgamestate.h \
    src/mousemoveableqmainwindow.h \
    src/aboutqmswidget.h \
    src/boardresizewidget.h \
    src/mousemoveableqwidget.h

SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/gamecontroller.cpp \
           src/minecoordinates.cpp \
    src/qmsapplicationsettings.cpp \
    src/qmsbutton.cpp \
    src/qmsicons.cpp \
    src/qmssettingsloader.cpp \
    src/qmssoundeffects.cpp \
    src/qmsstrings.cpp \
    src/qmsutilities.cpp \
    src/qmsgamestate.cpp \
    src/mousemoveableqmainwindow.cpp \
    src/aboutqmswidget.cpp \
    src/boardresizewidget.cpp \
    src/mousemoveableqwidget.cpp

RESOURCES += resources/icons.qrc \
             resources/sounds.qrc \
             resources/translations.qrc \
    resources/licenses.qrc

FORMS += forms/mainwindow.ui \
    forms/aboutqmswidget.ui \
    forms/boardresizewidget.ui

win32 {
    RC_FILE += resources/qminesweeper.rc
    HEADERS += src/version.h
}

android {
    DISTFILES += Android/AndroidManifest.xml \
                Android/gradle.properties \
                 Android/gradle/wrapper/gradle-wrapper.jar \
                 Android/gradlew \
                 Android/res/values/libs.xml \
                 Android/build.gradle \
                 Android/gradle/wrapper/gradle-wrapper.properties \
                 Android/gradlew.bat

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/Android
}
