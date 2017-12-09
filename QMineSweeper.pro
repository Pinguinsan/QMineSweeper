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

HEADERS += src/MainWindow.h \
           src/GameController.h \
           src/MineCoordinates.h \
           src/MineCoordinateHash.h \
           src/EventTimer.h \
    src/GlobalDefinitions.h \
    src/QmsApplicationSettings.h \
    src/QmsButton.h \
    src/QmsIcons.h \
    src/QmsSettingsLoader.h \
    src/QmsSoundEffects.h \
    src/QmsStrings.h \
    src/QmsUtilities.h \
    src/QmsGameState.h \
    src/MouseMoveableQMainWindow.h \
    src/AboutApplicationWidget.h \
    src/BoardResizeWidget.h \
    src/MouseMoveableQWidget.h

SOURCES += src/Main.cpp \
           src/MainWindow.cpp \
           src/GameController.cpp \
           src/MineCoordinates.cpp \
    src/QmsApplicationSettings.cpp \
    src/QmsButton.cpp \
    src/QmsIcons.cpp \
    src/QmsSettingsLoader.cpp \
    src/QmsSoundEffects.cpp \
    src/QmsStrings.cpp \
    src/QmsUtilities.cpp \
    src/QmsGameState.cpp \
    src/MouseMoveableQMainWindow.cpp \
    src/AboutApplicationWidget.cpp \
    src/BoardResizeWidget.cpp \
    src/MouseMoveableQWidget.cpp

RESOURCES += resources/icons.qrc \
             resources/sounds.qrc \
             resources/translations.qrc \
    resources/licenses.qrc

FORMS += forms/MainWindow.ui \
    forms/AboutApplicationWidget.ui \
    forms/BoardResizeWidget.ui

win32 {
    RC_FILE += resources/qminesweeper.rc
    HEADERS += src/version.h
}

