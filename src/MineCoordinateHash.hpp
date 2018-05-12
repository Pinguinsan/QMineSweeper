#ifndef QMINESWEEPER_MINECOORDINATEHASH_HPP
#define QMINESWEEPER_MINECOORDINATEHASH_HPP

#include "MineCoordinates.hpp"
#include <utility>

class MineCoordinateHash
{
public:
    /* operator() : Overloaded operator(), for use as a functor.
     * This allows the MineCoordinates class to be used in a std::unordered_map */
    std::size_t operator()(const MineCoordinates &mc) const
    {
        //return std::hash<std::pair<int, int>>()(std::make_pair(mc.X(), mc.Y()));
        return (std::hash<int>()(mc.X()) ^ (std::hash<int>()(mc.Y())));
    }
};


#endif // QMINESWEEPER_MINECOORDINATEHASH_HPP
