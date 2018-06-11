#include "QmsIcons.hpp"

#include <QString>
#include <QIcon>
#include "QmsStrings.hpp"

using namespace QmsStrings;

QmsIcons *applicationIcons{nullptr};

/* QmsIcons() : Constructor, setting up the QIcons used.
 * This thin wrapper class for the QIcons is necessary because all QObjects
 * must be instantiated and set up after a QApplication is instantiated, so using them
 * as static objects is not usable. Thus, this late binding is necessary */
QmsIcons::QmsIcons() :
    MINE_ICON_16{QIcon{MINE_ICON_16_PATH}},
    MINE_ICON_24{QIcon{MINE_ICON_24_PATH}},
    MINE_ICON_32{QIcon{MINE_ICON_32_PATH}},
    MINE_ICON_48{QIcon{MINE_ICON_48_PATH}},
    MINE_ICON_64{QIcon{MINE_ICON_64_PATH}},
    MINE_ICON_72{QIcon{MINE_ICON_72_PATH}},
    MINE_ICON_96{QIcon{MINE_ICON_96_PATH}},
    MINE_ICON_128{QIcon{MINE_ICON_128_PATH}},
    FACE_ICON_BIG_SMILEY{QIcon{FACE_ICON_BIG_SMILEY_PATH}},
    FACE_ICON_CRAZY{QIcon{FACE_ICON_CRAZY_PATH}},
    FACE_ICON_FROWNY{QIcon{FACE_ICON_FROWNY_PATH}},
    FACE_ICON_SLEEPY{QIcon{FACE_ICON_SLEEPY_PATH}},
    FACE_ICON_SMILEY{QIcon{FACE_ICON_SMILEY_PATH}},
    FACE_ICON_WINKY{QIcon{FACE_ICON_WINKY_PATH}},
    COUNT_MINES_0{QIcon{}},
    COUNT_MINES_1{QIcon{COUNT_MINES_1_PATH}},
    COUNT_MINES_2{QIcon{COUNT_MINES_2_PATH}},
    COUNT_MINES_3{QIcon{COUNT_MINES_3_PATH}},
    COUNT_MINES_4{QIcon{COUNT_MINES_4_PATH}},
    COUNT_MINES_5{QIcon{COUNT_MINES_5_PATH}},
    COUNT_MINES_6{QIcon{COUNT_MINES_6_PATH}},
    COUNT_MINES_7{QIcon{COUNT_MINES_7_PATH}},
    COUNT_MINES_8{QIcon{COUNT_MINES_8_PATH}},
    STATUS_ICON_FLAG{QIcon{STATUS_ICON_FLAG_PATH}},
    STATUS_ICON_QUESTION{QIcon{STATUS_ICON_QUESTION_PATH}},
    STATUS_ICON_FLAG_CHECK{QIcon{STATUS_ICON_FLAG_CHECK_PATH}},
    STATUS_ICON_FLAG_X{QIcon{STATUS_ICON_FLAG_X_PATH}}
{
    //Constructor
}

QmsIcons *QmsIcons::initializeInstance()
{
    if (applicationIcons == nullptr) {
        applicationIcons = new QmsIcons{};
    }
    return applicationIcons;
}

QmsIcons &QmsIcons::changeIconPalette(QIcon *icon) {
    //TODO: Implement
    Q_UNUSED(icon);
    return *this;
}

/* ~QmsIcons() : Destructor, empty by default */
QmsIcons::~QmsIcons()
{
    //Destructor
}
