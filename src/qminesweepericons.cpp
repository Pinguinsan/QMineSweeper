/***********************************************************************
*    qminesweepericons.cpp:                                            *
*    Class for holding instances of all icons used in QMineSweeper     *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/Pinguinsan/QMineSweeper                        *
*    This file holds the implementation of a QMineSweeperIcons class   *
*    QMineSweeperIcons holds instances of all icons used in the game,  *
*    allowing members to be called to set icons, rather than having to *
*    use a raw filename string to access the icon on disk              *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "qminesweepericons.h"

QMineSweeperIcons::QMineSweeperIcons() :
    MINE_ICON_16{QIcon{":/mine-icons/png/mine-icons/16-qminesweeper.png"}},
    MINE_ICON_24{QIcon{":/mine-icons/png/mine-icons/24-qminesweeper.png"}},
    MINE_ICON_32{QIcon{":/mine-icons/png/mine-icons/32-qminesweeper.png"}},
    MINE_ICON_48{QIcon{":/mine-icons/png/mine-icons/48-qminesweeper.png"}},
    MINE_ICON_64{QIcon{":/mine-icons/png/mine-icons/64-qminesweeper.png"}},
    MINE_ICON_72{QIcon{":/mine-icons/png/mine-icons/72-qminesweeper.png"}},
    MINE_ICON_96{QIcon{":/mine-icons/png/mine-icons/96-qminesweeper.png"}},
    MINE_ICON_128{QIcon{":/mine-icons/png/mine-icons/128-qminesweeper.png"}},
    FACE_ICON_BIG_SMILEY{QIcon{":/face-icons/png/face-icons/big-smiley-face.png"}},
    FACE_ICON_CRAZY{QIcon{":/face-icons/png/face-icons/crazy-face.png"}},
    FACE_ICON_FROWNY{QIcon{":/face-icons/png/face-icons/frowny-face.png"}},
    FACE_ICON_SLEEPY{QIcon{":/face-icons/png/face-icons/sleepy-face.png"}},
    FACE_ICON_SMILEY{QIcon{":/face-icons/png/face-icons/smiley-face.png"}},
    FACE_ICON_WINKY{QIcon{":/face-icons/png/face-icons/winky-face.png"}},
    COUNT_MINES_0{QIcon{}},
    COUNT_MINES_1{QIcon{":/surrounding-mine-icons/png/surrounding-mine-icons/1.png"}},
    COUNT_MINES_2{QIcon{":/surrounding-mine-icons/png/surrounding-mine-icons/2.png"}},
    COUNT_MINES_3{QIcon{":/surrounding-mine-icons/png/surrounding-mine-icons/3.png"}},
    COUNT_MINES_4{QIcon{":/surrounding-mine-icons/png/surrounding-mine-icons/4.png"}},
    COUNT_MINES_5{QIcon{":/surrounding-mine-icons/png/surrounding-mine-icons/5.png"}},
    COUNT_MINES_6{QIcon{":/surrounding-mine-icons/png/surrounding-mine-icons/6.png"}},
    COUNT_MINES_7{QIcon{":/surrounding-mine-icons/png/surrounding-mine-icons/7.png"}},
    COUNT_MINES_8{QIcon{":/surrounding-mine-icons/png/surrounding-mine-icons/8.png"}},
    STATUS_ICON_FLAG{QIcon{":/status-icons/png/status-icons/flag.png"}},
    STATUS_ICON_QUESTION{QIcon{":/status-icons/png/status-icons/question-mark.png"}},
    STATUS_ICON_FLAG_CHECK{QIcon{":/status-icons/png/status-icons/flag-check.png"}},
    STATUS_ICON_FLAG_X{QIcon{":/status-icons/png/status-icons/flag-x.png"}}
{
    //Constructor
}

QMineSweeperIcons::~QMineSweeperIcons()
{
    //Destructor
}
