/***********************************************************************
*    minecoordinatehash.h:                                             *
*    Custom hash function for the MineCoordinates class                *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/Pinguinsan/QMineSweeper                        *
*    This file contains the implementation of a helper hash function   *
*    MineCoordinateHash includes an overloaded operator() for use in   *
*    hashing, so the MineCoordinate object can be used as a key in the *
*    STL std::unordered_map<TKey, TValue> data structure               *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_MINECOORDINATEHASH_H
#define QMINESWEEPER_MINECOORDINATEHASH_H

#include "minecoordinates.h"

class MineCoordinateHash
{
public:
    std::size_t operator()(const MineCoordinates &mc) const
    {
        auto xHash = std::hash<int>()(mc.X());
        auto yHash = std::hash<int>()(mc.Y());
        return (xHash ^ (yHash<<1));
        return xHash;
    }
};


#endif // MINECOORDINATEHASH_H
