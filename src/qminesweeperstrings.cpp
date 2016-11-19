/***********************************************************************
*    qminesweeperstrings.cpp:                                          *
*    Class for holding instances of all strings used in QMineSweeper   *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/Pinguinsan/QMineSweeper                        *
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

#include "qminesweeperstrings.h"


QMineSweeperStrings::QMineSweeperStrings() :
    MAIN_WINDOW_TITLE{"QMineSweeper"},
    GENERIC_ERROR_MESSAGE{"Something has gone terribly wrong and the application must exit"},
    START_NEW_GAME_WINDOW_TITLE{"Start new game?"},
    START_NEW_GAME_PROMPT{"Are you sure you'd like to reset the current game?"},
    START_NEW_GAME_INSTRUCTION{"Click on a minesweeper button to begin"},
    CLOSE_APPLICATION_WINDOW_TITLE{"Quit QMineSweeper?"},
    CLOSE_APPLICATION_WINDOW_PROMPT{"Are you sure you'd like to quit?"},
    RESIZE_BOARD_DIALOG_BASE{"Are you sure you'd like to end the current game and start a new "},
    WIN_DIALOG_BASE{"You win! It took "},
    WIN_DIALOG_MIDDLE{" moves and your total play time was "},
    UNCOVERED_NON_MINE_STYLESHEET{"color: rgb(200, 170, 255); background-color: rgb(200, 170, 255);"},
    UNCOVERED_MINE_STYLESHEET{"color: rgb(255, 0, 0); background-color: rgb(255, 0, 0);"},
    CONFIGURATION_FILE_NAME{"qminesweeper.config"},
    #if (defined(_WIN32) || defined(__CYGWIN__))
        DEFAULT_CONFIGUATION_FILE{""},
    #else
        DEFAULT_CONFIGURATION_FILE{"~/.local/qminesweeper/qminesweeper.config"},
    #endif
    BACKUP_CONFIGURATION_FILE{"/usr/share/qminesweeper/qminesweeper.config"},
    LAST_CHANCE_CONFIGURATION_FILE{"/opt/GitHub/QMineSweeper/config/qminesweeper.config"},
    DEFAULT_PARAMETER_FILE_PATH{"config/defaults.config"}
{
    //Constructor
}

QMineSweeperStrings::~QMineSweeperStrings()
{
    //Destructor
}
