#ifndef QMINESWEEPER_QMSICONS_HPP
#define QMINESWEEPER_QMSICONS_HPP

#include <QIcon>
#include "../../../../../../Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.14.26428/include/memory"
#include "../../../../../../Qt/Tools/mingw492_32/i686-w64-mingw32/include/c++/bits/shared_ptr.h"

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

    static QmsIcons *initializeInstance();

    QmsIcons &changeIconPalette(QIcon *icon);

private:
    QmsIcons();
    ~QmsIcons();
    QmsIcons(const QmsIcons &) = delete;
    QmsIcons &operator=(const QmsIcons &rhs) = delete;
};

extern QmsIcons *applicationIcons;

#endif // QMINESWEEPER_QMSICONS_HPP
