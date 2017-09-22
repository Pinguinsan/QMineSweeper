/***********************************************************************
*    qmsicons.h:                                                       *
*    Class for holding instances of all icons used in QMineSweeper     *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file holds the declarations of a QmsIcons class              *
*    QmsIcons holds instances of all icons used in the game,           *
*    allowing members to be called to set icons, rather than having to *
*    use a raw filename string to access the icon on disk              *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_QMSICONS_H
#define QMINESWEEPER_QMSICONS_H

#include <QIcon>

class QmsIcons
{
public:
    const QIcon MINE_ICON_16;
    const QIcon MINE_ICON_24;
    const QIcon MINE_ICON_32;
    const QIcon MINE_ICON_48;
    const QIcon MINE_ICON_64;
    const QIcon MINE_ICON_72;
    const QIcon MINE_ICON_96;
    const QIcon MINE_ICON_128;

    const QIcon FACE_ICON_BIG_SMILEY;
    const QIcon FACE_ICON_CRAZY;
    const QIcon FACE_ICON_FROWNY;
    const QIcon FACE_ICON_SLEEPY;
    const QIcon FACE_ICON_SMILEY;
    const QIcon FACE_ICON_WINKY;

    const QIcon COUNT_MINES_0;
    const QIcon COUNT_MINES_1;
    const QIcon COUNT_MINES_2;
    const QIcon COUNT_MINES_3;
    const QIcon COUNT_MINES_4;
    const QIcon COUNT_MINES_5;
    const QIcon COUNT_MINES_6;
    const QIcon COUNT_MINES_7;
    const QIcon COUNT_MINES_8;

    const QIcon STATUS_ICON_FLAG;
    const QIcon STATUS_ICON_QUESTION;
    const QIcon STATUS_ICON_FLAG_CHECK;
    const QIcon STATUS_ICON_FLAG_X;

    static void initializeInstance();

private:
    QmsIcons();
    ~QmsIcons();
    QmsIcons(const QmsIcons &) = delete;
    QmsIcons &operator=(const QmsIcons &rhs) = delete;
};

extern QmsIcons *applicationIcons;

#endif // QMINESWEEPER_QMSRICONS_H
