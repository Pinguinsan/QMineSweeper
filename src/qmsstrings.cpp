/***********************************************************************
*    qmsstrings.cpp:                                          *
*    Class for holding instances of all strings used in QMineSweeper   *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the implementation of a QMineSweeperStrings class *
*    QMineSweeperStrings holds instances of all strings used in the    *
*    game, allowing members to be used, rather than having them        *
*    scattered throughout the other classes used in QMineSweeper       *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "qmsstrings.h"

namespace QmsStrings
{
    const char *MAIN_WINDOW_TITLE{"QMineSweeper"};
    const char *GENERIC_ERROR_MESSAGE{"Something has gone terribly wrong and the application must exit"};
    const char *START_NEW_GAME_WINDOW_TITLE{"Start new game?"};
    const char *START_NEW_GAME_PROMPT{"Are you sure you'd like to reset the current game?"};
    const char *START_NEW_GAME_INSTRUCTION{"Click on a minesweeper button to begin"};
    const char *CLOSE_APPLICATION_WINDOW_TITLE{"Quit QMineSweeper?"};
    const char *CLOSE_APPLICATION_WINDOW_PROMPT{"Are you sure you'd like to quit?"};
    const char *RESIZE_BOARD_WINDOW_CONFIRMATION_BASE{"Are you sure you'd like to end the current game and start a new "};
    const char *RESIZE_BOARD_WINDOW_CURRENT_BOARD_SIZE_STRING{"Current (columns x rows): "};
    const char *RESIZE_BOARD_WINDOW_CONFIRMATION_TAIL{" game?"};
    const char *RESIZE_BOARD_WINDOW_CONFIRMATION_MIDDLE{"x"};
    const char *WIN_DIALOG_BASE{"You win! It took "};
    const char *WIN_DIALOG_MIDDLE{" moves and your total play time was "};
    const char *UNCOVERED_NON_MINE_STYLESHEET{"color: rgb(200, 170, 255); background-color: rgb(200, 170, 255);"};
    const char *UNCOVERED_MINE_STYLESHEET{"color: rgb(255, 0, 0); background-color: rgb(255, 0, 0);"};
    const char *LONG_CLICKED_MINE_STYLESHEET{"color: rgb(0, 255, 0); background-color: rgb(0, 255, 0);"};
    const char *CONFIGURATION_FILE_NAME{"qms.config"};
    #if (defined(_WIN32) || defined(__CYGWIN__))
        const char *DEFAULT_CONFIGUATION_FILE{""};
    #else
        const char *DEFAULT_CONFIGURATION_FILE{"~/.local/qminesweeper/qminesweeper.config"};
    #endif
    const char *BACKUP_CONFIGURATION_FILE{"/usr/share/qminesweeper/qminesweeper.config"};
    const char *LAST_CHANCE_CONFIGURATION_FILE{"/opt/GitHub/QMineSweeper/config/qminesweeper.config"};
    const char *DEFAULT_PARAMETER_FILE_PATH{"config/defaults.config"};

    const char *LCD_OVERFLOW_STRING{"999"};
    const char *ENGLISH_STRING{"English"};
    const char *FRENCH_STRING{"French"};
    const char *SPANISH_STRING{"Spanish"};
    const char *JAPANESE_STRING{"Japanese"};

    const char *SAVED_GAME_FILE_EXTENSION{".qms"};
    const char *SAVE_FILE_CAPTION{"Save game?"};
    const char *OPEN_FILE_CAPTION{"Open existing game?"};

    const char *ABOUT_QT_WINDOW_TITLE{"About Qt"};

    const char *ENGLISH_TRANSLATION_PATH{":/translations/english.qm"};
    const char *FRENCH_TRANSLATION_PATH{":/translations/french.qm"};
    const char *SPANISH_TRANSLATION_PATH{":/translations/spanish.qm"};
    const char *JAPANESE_TRANSLATION_PATH{":/translations/japanese.qm"};

    const char *MINE_ICON_16_PATH{":/mine-icons/png/mine-icons/16-qminesweeper.png"};
    const char *MINE_ICON_24_PATH{":/mine-icons/png/mine-icons/24-qminesweeper.png"};
    const char *MINE_ICON_32_PATH{":/mine-icons/png/mine-icons/32-qminesweeper.png"};
    const char *MINE_ICON_48_PATH{":/mine-icons/png/mine-icons/48-qminesweeper.png"};
    const char *MINE_ICON_64_PATH{":/mine-icons/png/mine-icons/64-qminesweeper.png"};
    const char *MINE_ICON_72_PATH{":/mine-icons/png/mine-icons/72-qminesweeper.png"};
    const char *MINE_ICON_96_PATH{":/mine-icons/png/mine-icons/96-qminesweeper.png"};
    const char *MINE_ICON_128_PATH{":/mine-icons/png/mine-icons/128-qminesweeper.png"};
    const char *FACE_ICON_BIG_SMILEY_PATH{":/face-icons/png/face-icons/big-smiley-face.png"};
    const char *FACE_ICON_CRAZY_PATH{":/face-icons/png/face-icons/crazy-face.png"};
    const char *FACE_ICON_FROWNY_PATH{":/face-icons/png/face-icons/frowny-face.png"};
    const char *FACE_ICON_SLEEPY_PATH{":/face-icons/png/face-icons/sleepy-face.png"};
    const char *FACE_ICON_SMILEY_PATH{":/face-icons/png/face-icons/smiley-face.png"};
    const char *FACE_ICON_WINKY_PATH{":/face-icons/png/face-icons/winky-face.png"};
    const char *COUNT_MINES_0_PATH{};
    const char *COUNT_MINES_1_PATH{":/surrounding-mine-icons/png/surrounding-mine-icons/1.png"};
    const char *COUNT_MINES_2_PATH{":/surrounding-mine-icons/png/surrounding-mine-icons/2.png"};
    const char *COUNT_MINES_3_PATH{":/surrounding-mine-icons/png/surrounding-mine-icons/3.png"};
    const char *COUNT_MINES_4_PATH{":/surrounding-mine-icons/png/surrounding-mine-icons/4.png"};
    const char *COUNT_MINES_5_PATH{":/surrounding-mine-icons/png/surrounding-mine-icons/5.png"};
    const char *COUNT_MINES_6_PATH{":/surrounding-mine-icons/png/surrounding-mine-icons/6.png"};
    const char *COUNT_MINES_7_PATH{":/surrounding-mine-icons/png/surrounding-mine-icons/7.png"};
    const char *COUNT_MINES_8_PATH{":/surrounding-mine-icons/png/surrounding-mine-icons/8.png"};
    const char *STATUS_ICON_FLAG_PATH{":/status-icons/png/status-icons/flag.png"};
    const char *STATUS_ICON_QUESTION_PATH{":/status-icons/png/status-icons/question-mark.png"};
    const char *STATUS_ICON_FLAG_CHECK_PATH{":/status-icons/png/status-icons/flag-check.png"};
    const char *STATUS_ICON_FLAG_X_PATH{":/status-icons/png/status-icons/flag-x.png"};

    const char *EXPLOSION_EFFECT_SOURCE_PATH{"qrc:/sounds/sounds/explosion-fast.wav"};

    const char *QMINESWEEPER_LICENSE_PATH{":/licenses/licenses/LICENSE"};
}
