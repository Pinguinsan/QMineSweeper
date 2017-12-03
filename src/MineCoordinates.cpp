/***********************************************************************
*    minecoordinates.cpp:                                              *
*    Custom pair class that includes the x and y coordinates for mines *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
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

#include "MineCoordinates.h"

/* MineCoordinates() : Constructor for MineCoordinates using separate
 * x and y parameters, simply copying these elements to m_x and m_y */
MineCoordinates::MineCoordinates(int X, int Y) :
    m_x{X},
    m_y{Y}
{

}

/* X() : Member access to the x coordinate of the (x,y) coordinate pair */
int MineCoordinates::X() const
{
    return this->m_x;
}

/* Y() : Member access to the y coordinate of the (x,y) coordinate pair */
int MineCoordinates::Y() const
{
    return this->m_y;
}

/* setX() : Set the x coordinate of the (x,y) coordinate pair */
void MineCoordinates::setX(int X)
{
    this->m_x = X;
}

/* setY() : Set the y coordinate of the (x,y) coordinate pair */
void MineCoordinates::setY(int Y)
{
    this->m_y = Y;
}

/* operator==() : Overloaded operator==, checking equality for two MineCoordinates
 * instances, passing object by const reference.
 * Compares the X and Y components of the objects for exact equality*/
bool MineCoordinates::operator==(const MineCoordinates &compareObject) const
{
    return ((compareObject.X() == this->m_x) && (compareObject.Y() == this->m_y));
}


/* operator<() : Overloaded operator<, checking order of two MineCoordinates
 * instances, passing object by const reference.
 * Compares the X component of the objects to see if the current instance is
 * less than the passed in object*/
bool MineCoordinates::operator<(const MineCoordinates &compareObject) const
{
    if (this->m_x != compareObject.m_x) {
        return this->m_x < compareObject.m_x;
    }
    return (this->m_y < compareObject.m_y);
}
/* operator<=() : Overloaded operator<=, checking order of two MineCoordinates
 * instances, passing object by const reference.
 * Compares the X component of the objects to see if the current instance is
 * less than or equal to the passed in object */
bool MineCoordinates::operator<=(const MineCoordinates &compareObject) const
{
    return ( (*this < compareObject) || (*this == compareObject) );
}

/* operator>() : Overloaded operator>, checking order of two MineCoordinates
 * instances, passing object by const reference.
 * Compares the X component of the objects to see if the current instance is
 * greater than the passed in object */
bool MineCoordinates::operator>(const MineCoordinates &compareObject) const
{
    if (this->m_x != compareObject.m_x) {
        return this->m_x > compareObject.m_x;
    }
    return (this->m_y > compareObject.m_y);
}

/* operator>=() : Overloaded operator>=, checking order of two MineCoordinates
 * instances, passing object by const reference.
 * Compares the X component of the objects to see if the current instance is
 * greater than or equal to the passed in object */
bool MineCoordinates::operator>=(const MineCoordinates &compareObject) const
{
    return ( (*this > compareObject) || (*this == compareObject) );
}

/* toString() : A string representation of the coordinate pair */
std::string MineCoordinates::toString() const
{
    return QmsUtilities::TStringFormat("({0},{1})", this->m_x, this->m_y);
}

/* toQString() : A string representation of the coordinate pair */
QString MineCoordinates::toQString() const
{
    return QmsUtilities::QStringFormat("({0},{1})", this->m_x, this->m_y);
}

MineCoordinates MineCoordinates::parse(const std::string &str)
{
    auto foundComma = str.find(',');
    if (foundComma == std::string::npos) {

    }
    std::string rawFirst{str.substr(1, foundComma-1)};
    std::string rawSecond{str.substr(foundComma + 1)};
    if ( (rawSecond.length() > 0) && (rawSecond.back() == ')') ) {
        rawSecond.pop_back();
    }
    return MineCoordinates{std::stoi(rawFirst), std::stoi(rawSecond)};
}

/* toStdPair() : A std::pair<int, int> representation of the coordinate pair */
std::pair<int, int> MineCoordinates::toStdPair() const
{
    return std::make_pair(this->m_x, this->m_y);
}

/* operator<<() : Overloaded operator <<, enabling the pair to be output to
 * and output stream, passed in by const reference */
std::ostream& operator<<(std::ostream& os, const MineCoordinates &mc)
{
    return (os << mc.toString());
}

/* operator<<() : Overloaded operator <<, enabling the pair to be output to
 * and output stream, passed in by shared_ptr */
std::ostream& operator<<(std::ostream& os, std::shared_ptr<MineCoordinates> mc)
{
    return (os << mc->toString());
}
