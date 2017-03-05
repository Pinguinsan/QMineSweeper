/***********************************************************************
*    qminesweeperutilities.h:                                          *
*    Namespace for general utilities used by classes in QMineSweeper   *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for QMineSweeper:                           *
*    https://github.com/Pinguinsan/QMineSweeper                        *
*    This file holds the declarations for a QMineSweeperUtilities      *
*    namespace. QMineSweeperUtilities holds utilities shared by all    *
*    classes, such a delay functions, random functions, etc            *
*    The source code is released under the LGPL                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef QMINESWEEPER_QMINESWEEPERUTILITIES_H
#define QMINESWEEPER_QMINESWEEPERUTILITIES_H

#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <cstdio>

#include <QString>



namespace QMineSweeperUtilities
{
    void logString(const std::string &str);

    template<typename T>
    std::string toString(const T &convert)
    {
        std::string returnString{""};
        std::stringstream transfer;
        transfer << convert;
        transfer >> returnString;
        return returnString;
    }


    std::string toString(const std::string &str);
    std::string toString(const char *str);
    std::string getPadding(size_t howMuch, char padChar);
    std::string getPadding(size_t howMuch, const char *padString);
    std::string getPadding(size_t howMuch, const std::string &padString);

    template <typename T>
    QString toQString(const T &convert) { return QString::fromStdString(toString(convert)); }
    QString toQString(const std::string &convert);
    QString toQString(const char *convert);
    QString toQString(const QString &convert);

    int roundIntuitively(double numberToRound);
    int randomBetween(int lowLimit, int highLimit);
    bool endsWith(const std::string &stringToCheck, const std::string &matchString);
    bool endsWith(const std::string &stringToCheck, char matchChar);

    template<typename ... Args>
    std::string stringFormat(const std::string& format, Args ... args)
    {
        size_t size{std::snprintf(nullptr, 0, format.c_str(), args ...) + 1};
        std::unique_ptr<char[]> stringBuffer{new char[size]};
        snprintf(stringBuffer.get(), size, format.c_str(), args ...);
        return std::string{stringBuffer.get(), stringBuffer.get() + size - 1};
    }

    template<typename Container>
    bool isSwitch(const std::string &switchToCheck, const Container &switches) {
        std::string copyString{switchToCheck};
        std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
        for (auto &it : switches) {
            if ((copyString == static_cast<std::string>(it)) &&
                (copyString.length() == static_cast<std::string>(it).length()) &&
                (copyString.find(static_cast<std::string>(it)) == 0)) {
                return true;
            }
        }
        return false;
    }

    template<typename Container>
    bool isSwitch(const char *switchToCheck, const Container &switches) {
        return isSwitch(static_cast<std::string>(switchToCheck), switches);
    }

    template<typename Container>
    bool isEqualsSwitch(const std::string &switchToCheck, const Container &switches) {
        std::string copyString{switchToCheck};
        std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
        for (auto &it : switches) {
            std::string copySwitch{static_cast<std::string>(it) + "="};
            if ((copyString.find(static_cast<std::string>(it) + "=") == 0)) {
                return true;
            }
        }
        return false;
    }

    template<typename Container>
    bool isEqualsSwitch(const char *switchToCheck, const Container &switches) {
        return isEqualsSwitch(static_cast<std::string>(switchToCheck), switches);
    }


    const long long int constexpr NANOSECONDS_PER_MICROSECOND{1000};
    const long long int constexpr NANOSECONDS_PER_MILLISECOND{1000000};
    const long long int constexpr NANOSECONDS_PER_SECOND{1000000000};
    const long long int constexpr NANOSECONDS_PER_MINUTE{60000000000};
    const long long int constexpr NANOSECONDS_PER_HOUR{3600000000000};
    const long long int constexpr NANOSECONDS_PER_DAY {86400000000000};

    const long long int constexpr MICROSECONDS_PER_MILLISECOND{1000};
    const long long int constexpr MICROSECONDS_PER_SECOND{1000000};
    const long long int constexpr MICROSECONDS_PER_MINUTE{60000000};
    const long long int constexpr MICROSECONDS_PER_HOUR{3600000000};
    const long long int constexpr MICROSECONDS_PER_DAY{86400000000};

    const long long int constexpr MILLISECONDS_PER_SECOND{1000};
    const long long int constexpr MILLISECONDS_PER_MINUTE{60000};
    const long long int constexpr MILLISECONDS_PER_HOUR{3600000};
    const long long int constexpr MILLISECONDS_PER_DAY{86400000};

    const long long int constexpr SECONDS_PER_MINUTE{60};
    const long long int constexpr SECONDS_PER_HOUR{3600};

    const long long int constexpr MINUTES_PER_HOUR{60};
    const long long int constexpr MINUTES_PER_DAY{1440};

    const long long int constexpr HOURS_PER_DAY{24};
}

#endif //QMINESWEEPERUTILITIES_H
