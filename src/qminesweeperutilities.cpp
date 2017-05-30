/***********************************************************************
*    qminesweeperutilities.cpp                                         *
*    Namespace for general utilities used by classes in QMineSweeper   *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
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

    static std::unique_ptr<Random> randomDevice = std::unique_ptr<Random>{new Random()};

    Random::Random(std::mt19937::result_type seed) :
        m_randomEngine{seed}
    {

    }

    int Random::drawNumber(int min, int max)
    {
        return std::uniform_int_distribution<int>{min, max}(this->m_randomEngine);
    }

    int randomBetween(int lowLimit, int highLimit, bool lowInclusive, bool highInclusive)
    {
        int low{lowInclusive ? lowLimit : lowLimit + 1};
        int high{highInclusive ? highLimit : highLimit - 1};
        return randomDevice->drawNumber(low, high);
    }

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

    std::string getPadding(size_t howMuch, char padChar)
    {
        std::string returnString{""};
        for (size_t i = 0; i < howMuch; i++) {
            returnString += padChar;
        }
        return returnString;
    }

    std::string getPadding(size_t howMuch, const char *padString)
    {
        std::string returnString{""};
        for (size_t i = 0; i < howMuch; i++) {
            returnString += padString;
        }
        return returnString;
    }

    std::string getPadding(size_t howMuch, const std::string &padString)
    {
        std::string returnString{""};
        for (size_t i = 0; i < howMuch; i++) {
            returnString += padString;
        }
        return returnString;
    }

    int stringToInt(const std::string &str)
    {
        std::string copyString{""};
        std::copy_if(str.begin(), str.end(), std::back_inserter(copyString), [](char c) -> bool { return std::isdigit(c); });
        if (std::all_of(copyString.begin(), copyString.end(), [](char c) -> bool { return c == '0'; })) {
            return 0;
        }
        int returnValue{atoi(copyString.c_str())};
        if (!returnValue) {
            throw std::runtime_error("");
        } else {
            return returnValue;
        }
    }

    int stringToInt(const char *str)
    {
        return stringToInt(std::string{str});
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

    std::string TStringFormat(const char *formatting)
    {
        return std::string{formatting};
    }

    std::string stripFromString(const std::string &stringToStrip, const std::string &whatToStrip)
    {
        std::string returnString{stringToStrip};
        if (returnString.find(whatToStrip) == std::string::npos) {
            return returnString;
        }
        size_t foundPosition{stringToStrip.find(whatToStrip)};
        if (foundPosition == 0) {
            returnString = returnString.substr(whatToStrip.length());
        } else if (foundPosition == (returnString.length() - whatToStrip.length())) {
            returnString = returnString.substr(0, foundPosition);
        } else {
            returnString = returnString.substr(0, foundPosition) + returnString.substr(foundPosition+whatToStrip.length());
        }
        return returnString;
    }

    std::string stripFromString(const std::string &stringToStrip, char whatToStrip)
    {
        return stripFromString(stringToStrip, std::string(1, whatToStrip));
    }


    std::string stripAllFromString(const std::string &stringToStrip, const std::string &whatToStrip)
       {
           std::string returnString = stringToStrip;
           if (returnString.find(whatToStrip) == std::string::npos) {
               return returnString;
           }
           while (returnString.find(whatToStrip) != std::string::npos) {
               returnString = stripFromString(returnString, whatToStrip);
           }
           return returnString;
       }

       std::string stripAllFromString(const std::string &stringToStrip, char whatToStrip)
       {
           return stripAllFromString(stringToStrip, std::string(1, whatToStrip));
       }

}
