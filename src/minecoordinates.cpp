/***********************************************************************
*    minecoordinates.cpp:                                              *
*    Custom pair class that includes the x and y coordinates for mines *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/Pinguinsan/QMineSweeper                        *
*    This file contains the implementation of a custom pair class      *
*    MineCoordinates serve as the x and y (or row and column)          *
*    coordinates for mines, abstracted so as not use use a lot of      *
*    unnamed std::pair<int, int> data structures                       *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "minecoordinates.h"

MineCoordinates::MineCoordinates(int X, int Y) :
    m_x{X},
    m_y{Y}
{

}

MineCoordinates::MineCoordinates(const std::pair<int, int> &xy) :
    m_x{xy.first},
    m_y{xy.second}
{

}

int MineCoordinates::X() const
{
    return this->m_x;
}

int MineCoordinates::Y() const
{
    return this->m_y;
}

void MineCoordinates::setX(int X)
{
    this->m_x = X;
}

void MineCoordinates::setY(int Y)
{
    this->m_y = Y;
}

bool MineCoordinates::operator==(const MineCoordinates &compareObject) const
{
    return ((compareObject.X() == this->m_x) && (compareObject.Y() == this->m_y));
}

bool MineCoordinates::operator==(const std::shared_ptr<MineCoordinates> &compareObject) const
{
    return ((compareObject->X() == this->m_x) && (compareObject->Y() == this->m_y));
}

bool MineCoordinates::operator<(const MineCoordinates &compareObject) const
{
    return (compareObject.X() > this->m_x);
}

bool MineCoordinates::operator<(const std::shared_ptr<MineCoordinates> &compareObject) const
{
    return (compareObject->X() > this->m_x);
}

bool MineCoordinates::operator<=(const MineCoordinates &compareObject) const
{
    return (compareObject.X() >= this->m_x);
}

bool MineCoordinates::operator<=(const std::shared_ptr<MineCoordinates> &compareObject) const
{
    return (compareObject->X() >= this->m_x);
}

bool MineCoordinates::operator>(const MineCoordinates &compareObject) const
{
    return (compareObject.X() < this->m_x);
}

bool MineCoordinates::operator>(const std::shared_ptr<MineCoordinates> &compareObject) const
{
    return (compareObject->X() < this->m_x);
}

bool MineCoordinates::operator>=(const MineCoordinates &compareObject) const
{
    return (compareObject.X() <= this->m_x);
}

bool MineCoordinates::operator>=(const std::shared_ptr<MineCoordinates> &compareObject) const
{
    return (compareObject->X() <= this->m_x);
}

std::shared_ptr<MineCoordinates> MineCoordinates::generateRandomMineCoordinatesPtrInBounds(int maxColumn, int maxRow)
{
    using namespace QMineSweeperUtilities;
    return std::make_shared<MineCoordinates> (randomBetween(0, maxColumn), randomBetween(0, maxRow));
}

MineCoordinates MineCoordinates::generateRandomMineCoordinatesInBounds(int maxColumn, int maxRow)
{
    using namespace QMineSweeperUtilities;
    return MineCoordinates{randomBetween(0, maxColumn), randomBetween(0, maxRow)};
}

std::ostream& operator<<(std::ostream& os, const MineCoordinates &mc)
{
    os << "(" << mc.X() << ", " << mc.Y() << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<MineCoordinates> &mc)
{
    os << "(" << mc->X() << ", " << mc->Y() << ")";
    return os;
}
