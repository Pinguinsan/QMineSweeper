/***********************************************************************
*    qminesweeperstrings.h:                                            *
*    Class for holding instances of all strings used in QMineSweeper   *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the declarations of a QMineSweeperStrings class   *
*    QMineSweeperStrings holds instances of all strings used in the    *
*    game, allowing members to be used, rather than having them        *
*    scattered throughout the other classes used in QMineSweeper       *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_QMINESWEEPERSTRINGS_H
#define QMINESWEEPER_QMINESWEEPERSTRINGS_H

#include <string>

namespace QMineSweeperStrings
{
    extern const char *MAIN_WINDOW_TITLE;
    extern const char *GENERIC_ERROR_MESSAGE;
    extern const char *START_NEW_GAME_WINDOW_TITLE;
    extern const char *START_NEW_GAME_PROMPT;
    extern const char *START_NEW_GAME_INSTRUCTION;
    extern const char *CLOSE_APPLICATION_WINDOW_TITLE;
    extern const char *CLOSE_APPLICATION_WINDOW_PROMPT;
    extern const char *RESIZE_BOARD_WINDOW_CONFIRMATION_BASE;
    extern const char *RESIZE_BOARD_WINDOW_CURRENT_BOARD_SIZE_STRING;
    extern const char *RESIZE_BOARD_WINDOW_CONFIRMATION_TAIL;
    extern const char *RESIZE_BOARD_WINDOW_CONFIRMATION_MIDDLE;
    extern const char *WIN_DIALOG_BASE;
    extern const char *WIN_DIALOG_MIDDLE;
    extern const char *UNCOVERED_NON_MINE_STYLESHEET;
    extern const char *UNCOVERED_MINE_STYLESHEET;
    extern const char *LONG_CLICKED_MINE_STYLESHEET;
    extern const char *CONFIGURATION_FILE_NAME;
#if (defined(_WIN32) || defined(__CYGWIN__))
    extern const char *DEFAULT_CONFIGUATION_FILE;
#else
    extern const char *DEFAULT_CONFIGURATION_FILE;
#endif
    extern const char *BACKUP_CONFIGURATION_FILE;
    extern const char *LAST_CHANCE_CONFIGURATION_FILE;
    extern const char *DEFAULT_PARAMETER_FILE_PATH;

    extern const char *NUMBER_OF_SURROUND_MINES_NEGATIVE_STRING;
    extern const char *LESS_THAN_ZERO_STRING;
    extern const char *NUMBER_OF_SURROUNDING_MINES_TOO_LARGE_STRING;
    extern const char *GREATER_THAN_8_STRING;
    extern const char *COLUMN_INDEX_NEGATIVE_STRING;
    extern const char *ROW_INDEX_NEGATIVE_STRING;
    extern const char *STANDARD_EXCEPTION_CAUGHT_IN_ADD_MINESWEEPER_BUTTON_STRING;
    extern const char *LCD_OVERFLOW_STRING;

    extern const char *MINE_ICON_16_STRING;
    extern const char *MINE_ICON_24_STRING;
    extern const char *MINE_ICON_32_STRING;
    extern const char *MINE_ICON_48_STRING;
    extern const char *MINE_ICON_64_STRING;
    extern const char *MINE_ICON_72_STRING;
    extern const char *MINE_ICON_96_STRING;
    extern const char *MINE_ICON_128_STRING;
    extern const char *FACE_ICON_BIG_SMILEY_STRING;
    extern const char *FACE_ICON_CRAZY_STRING;
    extern const char *FACE_ICON_FROWNY_STRING;
    extern const char *FACE_ICON_SLEEPY_STRING;
    extern const char *FACE_ICON_SMILEY_STRING;
    extern const char *FACE_ICON_WINKY_STRING;
    extern const char *COUNT_MINES_0_STRING;
    extern const char *COUNT_MINES_1_STRING;
    extern const char *COUNT_MINES_2_STRING;
    extern const char *COUNT_MINES_3_STRING;
    extern const char *COUNT_MINES_4_STRING;
    extern const char *COUNT_MINES_5_STRING;
    extern const char *COUNT_MINES_6_STRING;
    extern const char *COUNT_MINES_7_STRING;
    extern const char *COUNT_MINES_8_STRING;
    extern const char *STATUS_ICON_FLAG_STRING;
    extern const char *STATUS_ICON_QUESTION_STRING;
    extern const char *STATUS_ICON_FLAG_CHECK_STRING;
    extern const char *STATUS_ICON_FLAG_X_STRING;

    extern const char *EXPLOSION_EFFECT_SOURCE_STRING;
}

#endif //QMINESWEEPER_QMINESWEEPERSTRINGS_H
