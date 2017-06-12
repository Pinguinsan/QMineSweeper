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

#ifndef QMINESWEEPER_QMSSTRINGS_H
#define QMINESWEEPER_QMSSTRINGS_H

#include <string>

namespace QmsStrings
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

    extern const char *ENGLISH_STRING;
    extern const char *FRENCH_STRING;
    extern const char *SPANISH_STRING;
    extern const char *JAPANESE_STRING;

    extern const char *ENGLISH_TRANSLATION_PATH;
    extern const char *FRENCH_TRANSLATION_PATH;
    extern const char *SPANISH_TRANSLATION_PATH;
    extern const char *JAPANESE_TRANSLATION_PATH;

    extern const char *MINE_ICON_16_PATH;
    extern const char *MINE_ICON_24_PATH;
    extern const char *MINE_ICON_32_PATH;
    extern const char *MINE_ICON_48_PATH;
    extern const char *MINE_ICON_64_PATH;
    extern const char *MINE_ICON_72_PATH;
    extern const char *MINE_ICON_96_PATH;
    extern const char *MINE_ICON_128_PATH;
    extern const char *FACE_ICON_BIG_SMILEY_PATH;
    extern const char *FACE_ICON_CRAZY_PATH;
    extern const char *FACE_ICON_FROWNY_PATH;
    extern const char *FACE_ICON_SLEEPY_PATH;
    extern const char *FACE_ICON_SMILEY_PATH;
    extern const char *FACE_ICON_WINKY_PATH;
    extern const char *COUNT_MINES_0_PATH;
    extern const char *COUNT_MINES_1_PATH;
    extern const char *COUNT_MINES_2_PATH;
    extern const char *COUNT_MINES_3_PATH;
    extern const char *COUNT_MINES_4_PATH;
    extern const char *COUNT_MINES_5_PATH;
    extern const char *COUNT_MINES_6_PATH;
    extern const char *COUNT_MINES_7_PATH;
    extern const char *COUNT_MINES_8_PATH;
    extern const char *STATUS_ICON_FLAG_PATH;
    extern const char *STATUS_ICON_QUESTION_PATH;
    extern const char *STATUS_ICON_FLAG_CHECK_PATH;
    extern const char *STATUS_ICON_FLAG_X_PATH;

    extern const char *EXPLOSION_EFFECT_SOURCE_PATH;

    extern const char *QMINESWEEPER_LICENSE_PATH;
}

#endif //QMINESWEEPER_QMSSTRINGS_H
