/***********************************************************************
*    qminesweepericons.cpp:                                            *
*    Class for holding instances of all icons used in QMineSweeper     *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the implementation of a QmsIcons class            *
*    QmsIcons holds instances of all icons used in the game,           *
*    allowing members to be called to set icons, rather than having to *
*    use a raw filename string to access the icon on disk              *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "qmsicons.h"

#include <QString>
#include <QIcon>
#include "qmsstrings.h"

using namespace QmsStrings;

/* QmsIcons() : Constructor, setting up the QIcons used.
 * This thin wrapper class for the QIcons is necessary because all QObjects
 * must be instantiated and set up after a QApplication is instantiated, so using them
 * as static objects is not usable. Thus, this late binding is necessary */
QmsIcons::QmsIcons() :
    MINE_ICON_16{QIcon{MINE_ICON_16_STRING}},
    MINE_ICON_24{QIcon{MINE_ICON_24_STRING}},
    MINE_ICON_32{QIcon{MINE_ICON_32_STRING}},
    MINE_ICON_48{QIcon{MINE_ICON_48_STRING}},
    MINE_ICON_64{QIcon{MINE_ICON_64_STRING}},
    MINE_ICON_72{QIcon{MINE_ICON_72_STRING}},
    MINE_ICON_96{QIcon{MINE_ICON_96_STRING}},
    MINE_ICON_128{QIcon{MINE_ICON_128_STRING}},
    FACE_ICON_BIG_SMILEY{QIcon{FACE_ICON_BIG_SMILEY_STRING}},
    FACE_ICON_CRAZY{QIcon{FACE_ICON_CRAZY_STRING}},
    FACE_ICON_FROWNY{QIcon{FACE_ICON_FROWNY_STRING}},
    FACE_ICON_SLEEPY{QIcon{FACE_ICON_SLEEPY_STRING}},
    FACE_ICON_SMILEY{QIcon{FACE_ICON_SMILEY_STRING}},
    FACE_ICON_WINKY{QIcon{FACE_ICON_WINKY_STRING}},
    COUNT_MINES_0{QIcon{}},
    COUNT_MINES_1{QIcon{COUNT_MINES_1_STRING}},
    COUNT_MINES_2{QIcon{COUNT_MINES_2_STRING}},
    COUNT_MINES_3{QIcon{COUNT_MINES_3_STRING}},
    COUNT_MINES_4{QIcon{COUNT_MINES_4_STRING}},
    COUNT_MINES_5{QIcon{COUNT_MINES_5_STRING}},
    COUNT_MINES_6{QIcon{COUNT_MINES_6_STRING}},
    COUNT_MINES_7{QIcon{COUNT_MINES_7_STRING}},
    COUNT_MINES_8{QIcon{COUNT_MINES_8_STRING}},
    STATUS_ICON_FLAG{QIcon{STATUS_ICON_FLAG_STRING}},
    STATUS_ICON_QUESTION{QIcon{STATUS_ICON_QUESTION_STRING}},
    STATUS_ICON_FLAG_CHECK{QIcon{STATUS_ICON_FLAG_CHECK_STRING}},
    STATUS_ICON_FLAG_X{QIcon{STATUS_ICON_FLAG_X_STRING}}
{
    //Constructor
}

/* ~QmsIcons() : Destructor, empty by default */
QmsIcons::~QmsIcons()
{
    //Destructor
}
