/***********************************************************************
*    minecoordinatehash.h:                                             *
*    Custom hash function for the MineCoordinates class                *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
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
    /* operator() : Overloaded operator(), for use as a functor.
     * This allows the MineCoordinates class to be used in a std::unordered_map */
    std::size_t operator()(const MineCoordinates &mc) const
    {
        return (std::hash<int>()(mc.X()) ^ (std::hash<int>()(mc.Y())<<1));
    }
};


#endif // QMINESWEEPER_MINECOORDINATEHASH_H
