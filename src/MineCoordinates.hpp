#ifndef QMINESWEEPER_MINECOORDINATES_HPP
#define QMINESWEEPER_MINECOORDINATES_HPP

#include <functional>
#include <algorithm>
#include <memory>
#include <string>
#include <iostream>
#include <tuple>

#include "QmsUtilities.hpp"

class MineCoordinates
{

public:
    explicit MineCoordinates(int X, int Y);

    int X() const;
    void setX(int X);

    int Y() const;
    void setY(int Y);

    bool operator==(const MineCoordinates &compareObject) const;
    bool operator<(const MineCoordinates &compareObject) const;
    bool operator<=(const MineCoordinates &compareObject) const;
    bool operator>(const MineCoordinates &compareObject) const;
    bool operator>=(const MineCoordinates &compareObject) const;
    std::string toString() const;
    QString toQString() const;
    std::pair<int, int> toStdPair() const;
    friend std::ostream& operator<<(std::ostream &os, const MineCoordinates &mc);

    static MineCoordinates parse(const std::string &str);

private:
    int m_x;
    int m_y;

};

#endif //QMINESWEEPER_MINECOORDINATES_HPP
