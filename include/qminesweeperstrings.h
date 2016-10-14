/***********************************************************************
*    qminesweeperstrings.h:                                            *
*    Class for holding instances of all strings used in QMineSweeper   *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/Pinguinsan/QMineSweeper                        *
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

#ifndef QMINESWEEPERSTRINGS_H
#define QMINESWEEPERSTRINGS_H


class QMineSweeperStrings
{
public:
    QMineSweeperStrings();
    ~QMineSweeperStrings();
    const char *MAIN_WINDOW_TITLE;
    const char *GENERIC_ERROR_MESSAGE;
    const char *START_NEW_GAME_WINDOW_TITLE;
    const char *START_NEW_GAME_PROMPT;
    const char *START_NEW_GAME_INSTRUCTION;
    const char *CLOSE_APPLICATION_WINDOW_TITLE;
    const char *CLOSE_APPLICATION_WINDOW_PROMPT;
    const char *RESIZE_BOARD_DIALOG_BASE;
    const char *WIN_DIALOG_BASE;
    const char *WIN_DIALOG_MIDDLE;
    const char *UNCOVERED_NON_MINE_STYLESHEET;
    const char *UNCOVERED_MINE_STYLESHEET;
    const char *CONFIGURATION_FILE_NAME;
#if (defined(_WIN32) || defined(__CYGWIN__))
    const std::string DEFAULT_CONFIGUATION_FILE;
#else
    const char *DEFAULT_CONFIGURATION_FILE;
#endif
    const char *BACKUP_CONFIGURATION_FILE;
    const char *LAST_CHANCE_CONFIGURATION_FILE;
    const char *DEFAULT_PARAMETER_FILE_PATH;
};

#endif // QMINESWEEPERSTRINGS_H
