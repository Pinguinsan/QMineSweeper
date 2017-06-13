/***********************************************************************
*    minecoordinates.h:                                                *
*    Custom pair class that includes the x and y coordinates for mines *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    This file contains the declarations for a custom pair class       *
*    MineCoordinates serve as the x and y (or row and column)          *
*    coordinates for mines, abstracted so as not use use a lot of      *
*    unnamed std::pair<int, int> data structures                       *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_MINECOORDINATES_H
#define QMINESWEEPER_MINECOORDINATES_H

#include <functional>
#include <algorithm>
#include <memory>
#include <string>
#include <iostream>
#include <tuple>

#include "qmsutilities.h"

class MineCoordinates
{

public:
    MineCoordinates(int X, int Y);
    MineCoordinates(const std::pair<int, int> &xy);

    int X() const;
    void setX(int X);

    int Y() const;
    void setY(int Y);

    bool operator==(const MineCoordinates &compareObject) const;
    bool operator==(std::shared_ptr<MineCoordinates> compareObject) const;
    bool operator<(const MineCoordinates &compareObject) const;
    bool operator<(std::shared_ptr<MineCoordinates> compareObject) const;
    bool operator<=(const MineCoordinates &compareObject) const;
    bool operator<=(std::shared_ptr<MineCoordinates> compareObject) const;
    bool operator>(const MineCoordinates &compareObject) const;
    bool operator>(std::shared_ptr<MineCoordinates> compareObject) const;
    bool operator>=(const MineCoordinates &compareObject) const;
    bool operator>=(std::shared_ptr<MineCoordinates> compareObject) const;
    std::string toString() const;
    QString toQString() const;
    std::pair<int, int> toStdPair() const;
    friend std::ostream& operator<<(std::ostream &os, const MineCoordinates &mc);
    friend std::ostream& operator<<(std::ostream &os, std::shared_ptr<MineCoordinates> mc);

private:
    int m_x;
    int m_y;

};

#endif //QMINESWEEPER_MINECOORDINATES_H
