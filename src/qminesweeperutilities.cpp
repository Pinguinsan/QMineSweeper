/***********************************************************************
*    qminesweeperutilities.cpp                                         *
*    Namespace for general utilities used by classes in QMineSweeper   *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/Pinguinsan/QMineSweeper                        *
*    This file holds the implementation for a QMineSweeperUtilities    *
*    namespace. QMineSweeperUtilities holds utilities shared by all    *
*    classes, such a delay functions, random functions, etc            *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "qminesweeperutilities.h"

namespace QMineSweeperUtilities
{
    void logString(const std::string &str) { std::cout << str << std::endl; }
    std::string toString(const std::string &str) { return str; }
    std::string toString(const char *str) { return static_cast<std::string>(str); }

    QString toQString(const std::string &convert) { return QString::fromStdString(convert); }
    QString toQString(const char *convert) { return QString::fromStdString(static_cast<std::string>(convert)); }
    QString toQString(const QString &convert) { return convert; }

    int roundIntuitively(double numberToRound)
    {
        double tempContainer{numberToRound - static_cast<int>(numberToRound)};
        if (tempContainer >= 0.5) {
            return (static_cast<int>(numberToRound) + 1);
        } else {
            return static_cast<int>(numberToRound);
        }
    }

    int randomBetween(int lowLimit, int highLimit)
    {
        static bool srandSeeded{false};
        if (!srandSeeded) {
            srand(time(NULL));
            srandSeeded = true;
        }
        const int divisor{(RAND_MAX/highLimit) + 1};
        int returnValue{lowLimit - 1};
        do {
            returnValue = (rand() / divisor);
        } while ((returnValue > highLimit) || (returnValue < lowLimit));
        return returnValue;
    }

    bool endsWith(const std::string &stringToCheck, const std::string &matchString)
    {
        if (matchString.size() > stringToCheck.size()) {
            return false;
        }
        return std::equal(matchString.rbegin(), matchString.rend(), stringToCheck.rbegin());
    }

    bool endsWith(const std::string &stringToCheck, char matchChar)
    {
        return endsWith(stringToCheck, std::string{1, matchChar});
    }

}
